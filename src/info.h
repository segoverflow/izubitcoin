// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#ifndef _IZUBITCOIN_INFO_H
#define _IZUBITCOIN_INFO_H 1

#include "connection.h"
#include "message.h"
#include "options.h"

class Info {
    public:
        Info(Connection *connection, Options *options);
        void run();

    protected:
        Connection *connection;
        Options *options;
        Message *createMsgOut();
};

#endif
