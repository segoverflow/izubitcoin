// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "info.h"
#include "connection.h"
#include "message.h"
#include "utils.h"
#include "payloads/version.h"
#include <iostream>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>

Info::Info(Connection *connection) {
    this->connection = connection;
}

void Info::run() {
    // create msg out
    Message *msgOut = createMsgOut();
    
    // send msg out
    connection->sendMessage(msgOut);
    
    // receive msg in
    Message *msgIn = connection->recvMessage();
    
    // minimal checking
    if (
        (msgIn->getCommandName() != "version") ||
        (msgIn->getPayloadSize() < (sizeof(struct VersionPayload_1) + sizeof(struct VersionPayload_2))) ||
        !msgIn->getPayload()
    ) {
        Utils::exitError("Error receiving version message");
    }
    
    struct VersionPayload_1 *mvp1 = (struct VersionPayload_1 *) msgIn->getPayload();
    char userAgentBuf[512];
        
    // user agent size checking
    if (
        (mvp1->user_agent_bytes != (msgIn->getPayloadSize() - sizeof(struct VersionPayload_1) - sizeof(struct VersionPayload_2))) ||
        (mvp1->user_agent_bytes >= sizeof(userAgentBuf))
    ) {
        Utils::exitError("Corrupted version message");
    }
        
    // copy user agent to buffer
    char *ua = (char *) msgIn->getPayload() + sizeof(struct VersionPayload_1);
    memset(userAgentBuf, 0, sizeof(userAgentBuf));
    memcpy(userAgentBuf, (void *) ua, mvp1->user_agent_bytes);
        
    cout << "Bitcoin node version: " << userAgentBuf << endl;
}

Message *Info::createMsgOut() {
    struct VersionPayload_1 vp1;
    struct VersionPayload_2 vp2;
            
    // structs zeroed
    memset(&vp1, 0, sizeof(vp1));
    memset(&vp2, 0, sizeof(vp2));
        
    vp1.version = NODE_VERSION;
    vp1.services = 0;
    vp1.timestamp = time(NULL);
    
    // recv address
    vp1.addr_recv_services = 0;
    memcpy(vp1.addr_recv_ip_address, connection->ipTargetAddr(), 16);
    vp1.addr_recv_port = htons(connection->getTargetPort());
    
    // trans address
    vp1.addr_trans_services = 0;
    memset(vp1.addr_trans_ip_address, 0x00, 10);
    memset(&vp1.addr_trans_ip_address[10], 0xff, 2);
    *((uint32_t *) &vp1.addr_trans_ip_address[12]) = htonl(0x7f000001);  // 127.0.0.1
    vp1.addr_trans_port = htons(connection->getSourcePort());
    
    vp1.nonce = 0;
    vp1.user_agent_bytes = strlen(USER_AGENT);
    vp2.start_height = 1;
    vp2.relay = 0;
    
    // construct full version payload
    uint8_t *vp = (uint8_t *) malloc(sizeof(vp1) + sizeof(vp2) + vp1.user_agent_bytes);
    
    if (vp == NULL) {
        Utils::exitError("malloc error");
    }
    
    memcpy(vp, &vp1, sizeof(vp1));
    memcpy(vp + sizeof(vp1), USER_AGENT, vp1.user_agent_bytes);
    memcpy(vp + sizeof(vp1) + vp1.user_agent_bytes, &vp2, sizeof(vp2));
    
    // create msg out
    Message *msgOut = new Message(Main);
    msgOut->setCommandName("version");
    msgOut->setPayload((u_char *) vp, sizeof(vp1) + sizeof(vp2) + vp1.user_agent_bytes);
    
    return msgOut;
}
