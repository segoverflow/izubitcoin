// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#ifndef _IZUBITCOIN_CONNECTION_H
#define _IZUBITCOIN_CONNECTION_H 1

#include <string>
#include <netdb.h>
#include "message.h"

using namespace std;

class Connection {
    public:
        Connection(string target, int port);
        void sendMessage(Message *msg);
        Message *recvMessage();
        char *ipTargetAddr();
        int getTargetPort();
        int getSourcePort();

    protected:
        const string targetStr;
        const int targetPort;
        struct sockaddr *targetSockaddr;
        int sockfd;
};

#endif
