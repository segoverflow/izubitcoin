// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#ifndef _IZUBITCOIN_UTILS_H
#define _IZUBITCOIN_UTILS_H 1

#include <getopt.h>
#include <string>
#include "connection.h"
#include "options.h"

using namespace std;

class Utils {
    public:
        static void initApp(int argc, char *argv[]);
        static void exitError(string msg);

    protected:
        static const string optstring;
        static const struct option long_options[];

        static void printVersion(void);
        static void printHelp(void);
        static void runCommands(string commands, Connection &connection, Options &options);
};

#endif
