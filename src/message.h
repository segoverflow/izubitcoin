// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#ifndef _IZUBITCOIN_MESSAGE_H
#define _IZUBITCOIN_MESSAGE_H 1

#include <string>

using namespace std;

enum Network {
    Main = 0xD9B4BEF9,
    Testnet = 0xDAB5BFFA
};

struct MessageRaw {
    void *content;
    size_t size;
};

struct MessageHeader {
    uint32_t magic;
    char command[12];
    uint32_t payloadLength;
    uint32_t checksum;
} __attribute__ ((__packed__));

class Message {
    public:
        Message(Network network);
        string getCommandName();
        size_t getPayloadSize();
        u_char *getPayload();
        void setCommandName(string commandName);
        void setPayload(u_char *payload, size_t size);
        uint32_t checksum();
        struct MessageRaw raw();

    protected:
        Network network;
        string commandName;
        u_char *payload;
        size_t payloadSize;
};

#endif
