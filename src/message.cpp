// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "message.h"
#include "utils.h"
#include <openssl/sha.h>
#include <string.h>
#include <iostream>

using namespace std;

Message::Message(Network network) : payloadSize(0), payload(NULL) {
    this->network = network;
}

uint32_t Message::checksum() {
    unsigned char digest[SHA256_DIGEST_LENGTH];

    // first SHA256
    if (payloadSize) {
        SHA256(payload, payloadSize, (unsigned char *) digest);
    } else {
        SHA256((unsigned char *) "", 0, (unsigned char *) digest);
    }
    
    // second SHA256
    SHA256(digest, SHA256_DIGEST_LENGTH, (unsigned char *) digest);

    return (*(uint32_t *) digest);
}

string Message::getCommandName() {
    return this->commandName;
}

size_t Message::getPayloadSize() {
    return this->payloadSize;
}

u_char *Message::getPayload() {
    return this->payload;
}

void Message::setCommandName(string commandName) {
    this->commandName = commandName;
}

void Message::setPayload(u_char *payload, size_t payloadSize) {
    this->payload = payload;
    this->payloadSize = payloadSize;
}

struct MessageRaw Message::raw() {
    struct MessageRaw msgRaw;
    struct MessageHeader *msgHeader;
    
    msgRaw.size = sizeof(struct MessageHeader) + payloadSize;
    msgRaw.content = (void *) malloc(msgRaw.size);
    
    if (msgRaw.content == NULL) {
        Utils::exitError("malloc error");
    }
    
    // zeroed
    memset(msgRaw.content, 0, msgRaw.size);
    
    // header
    msgHeader = (struct MessageHeader *) msgRaw.content;
    msgHeader->magic = network;
    strncpy(msgHeader->command, commandName.c_str(), commandName.size());
    msgHeader->payloadLength = payloadSize;
    msgHeader->checksum = checksum();
    
    // payload
    memcpy((char *) msgRaw.content + sizeof(struct MessageHeader), payload, payloadSize);
                        
    return (msgRaw);
}
