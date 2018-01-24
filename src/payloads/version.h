// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#ifndef _IZUBITCOIN_PAYLOAD_VERSION_H
#define _IZUBITCOIN_PAYLOAD_VERSION_H 1

struct VersionPayload_1 {
    int32_t version;
    uint64_t services;
    uint64_t timestamp;
    uint64_t addr_recv_services;
    char addr_recv_ip_address[16];
    uint16_t addr_recv_port;
    uint64_t addr_trans_services;
    char addr_trans_ip_address[16];
    uint16_t addr_trans_port;
    uint64_t nonce;
    uint8_t user_agent_bytes;
} __attribute__ ((__packed__));

// user agent value (var length) is between VersionPayload_1 and VersionPayload_2

struct VersionPayload_2 {
    int32_t start_height;
    int8_t relay;
} __attribute__ ((__packed__));

#endif
