// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "connection.h"
#include "message.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

Connection::Connection(string target, int port) : targetStr(target), targetPort(port), targetSockaddr(NULL) {
    struct addrinfo hint, *ailist, *aip;
    int error;

    // initialize hint
    hint.ai_flags = AI_NUMERICSERV;
    hint.ai_family = 0;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_addrlen = 0;
    hint.ai_addr = NULL;
    hint.ai_canonname = NULL;
    hint.ai_next = NULL;

    if (error = getaddrinfo(target.c_str(), to_string(port).c_str(), &hint, &ailist)) {
        Utils::exitError("Error getaddrinfo: " + string(gai_strerror(error)));
    }

    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((aip->ai_family == AF_INET) || (aip->ai_family == AF_INET6)) {
            targetSockaddr = aip->ai_addr;

            // creating socket
            if ((sockfd = socket(aip->ai_family, SOCK_STREAM, IPPROTO_TCP)) == -1) {
                Utils::exitError("Error creating socket");
            }
            
            // setting non-blocking
            fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, NULL) | O_NONBLOCK);
        
            // connecting to node
            if (connect(sockfd, targetSockaddr, aip->ai_addrlen) == -1) {
                if (errno != EINPROGRESS) {
                    Utils::exitError("Error connecting to node " + target + ":" + to_string(port) + " - " + strerror(errno));
                } else {
                    // still connection in progress
                    struct timeval tv;
                    fd_set wfdset;

                    FD_ZERO(&wfdset);
                    FD_SET(sockfd, &wfdset);
                    tv.tv_sec = CONNECTION_TIMEOUT;
                    tv.tv_usec = 0;
                    
                    if (select(sockfd + 1, NULL, &wfdset, NULL, &tv) > 0) {
                        socklen_t len = sizeof error;
                                                
                        // checking socket error
                        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);

                        if (error) {
                            Utils::exitError("Error connecting to node " + target + ":" + to_string(port) + " - " + strerror(error));
                        }
                    } else {
                        Utils::exitError("Error connecting to node " + target + ":" + to_string(port));
                    }
                }
            }

            // success connection
            cout << "Connected to " << target << ":" << to_string(port) << endl;
            return;
        }
    }

    Utils::exitError("Error finding addrinfo");
}

void Connection::sendMessage(Message *msg) {
    struct MessageRaw msgRaw = msg->raw();
    struct timeval tv;
    fd_set wfdset;

    FD_ZERO(&wfdset);
    FD_SET(sockfd, &wfdset);
    tv.tv_sec = SEND_MSG_TIMEOUT;
    tv.tv_usec = 0;
    
    if (select(sockfd + 1, NULL, &wfdset, NULL, &tv) > 0) {
        send(sockfd, msgRaw.content, msgRaw.size, 0);
    } else {
        Utils::exitError("Error sending message to node " + targetStr + ":" + to_string(targetPort));
    }
}

Message *Connection::recvMessage() {
    struct timeval tv;
    fd_set rfdset;

    FD_ZERO(&rfdset);
    FD_SET(sockfd, &rfdset);
    tv.tv_sec = RECV_MSG_TIMEOUT;
    tv.tv_usec = 0;
    
    if (select(sockfd + 1, &rfdset, NULL, NULL, &tv) > 0) {
        struct MessageHeader msgHeader;
        u_char *payload;
        Message *msg;
        
        // header zeroed
        memset(&msgHeader, 0, sizeof(struct MessageHeader));
        
        // receive header
        recv(sockfd, &msgHeader, sizeof(struct MessageHeader), 0);

        // instantiate message
        msg = new Message((Network) msgHeader.magic);
        msg->setCommandName(msgHeader.command);
            
        // payload
        if (msgHeader.payloadLength) {
            if ((payload = (u_char *) malloc(msgHeader.payloadLength)) == NULL) {
                Utils::exitError("malloc error");
            }
            
            memset(payload, 0, msgHeader.payloadLength);
            recv(sockfd, payload, msgHeader.payloadLength, 0);
            msg->setPayload(payload, msgHeader.payloadLength);
        }
        
        return msg;
    } else {
        Utils::exitError("Error receiving message from node " + targetStr + ":" + to_string(targetPort));
    }
}

char *Connection::ipTargetAddr() {
    char *ip = (char *) malloc(16);
    
    if (ip == NULL) {
        Utils::exitError("malloc error");
    }
    
    switch (targetSockaddr->sa_family) {
        case AF_INET: {
            struct sockaddr_in *addr_in = (struct sockaddr_in *) targetSockaddr;
            
            // IPv4 mapped IPv6
            memset(ip, 0x00, 10);
            memset(&ip[10], 0xff, 2);
            memcpy(&ip[12], &addr_in->sin_addr, 4);
            
            return ip;
        }
        case AF_INET6: {
            struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *) targetSockaddr;
            memcpy(ip, &addr_in6->sin6_addr, 16);
                        
            return ip;
        }
        default:
            Utils::exitError("Error getting ip target addr");
    }
}

int Connection::getTargetPort() {
    return targetPort;
}

int Connection::getSourcePort() {
    uint8_t buf[64];
    socklen_t saddrSize = sizeof(buf);
    struct sockaddr *saddr = (struct sockaddr *) buf;
        
    if (getsockname(sockfd, saddr, &saddrSize) || (saddrSize > sizeof(buf))) {
        Utils::exitError("Error getting source port (getsockname)");
    }
    
    switch (saddr->sa_family) {
        case AF_INET: {
            struct sockaddr_in *addr_in = (struct sockaddr_in *) saddr;
                        
            return ntohs(addr_in->sin_port);
        }
        case AF_INET6: {
            struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *) saddr;
                        
            return ntohs(addr_in6->sin6_port);
        }
        default:
            Utils::exitError("Error getting source port");
    }
}
