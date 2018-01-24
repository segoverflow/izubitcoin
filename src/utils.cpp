// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "utils.h"
#include "connection.h"
#include "info.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

using namespace std;

const string Utils::optstring = "hvp:i";

const struct option Utils::long_options[] = {
    { "help", no_argument, 0, 'h' },
    { "version", no_argument, 0, 'v' },
    { "port", required_argument, 0, 'p' },
    { "info", no_argument, 0, 'i' },
    { 0, 0, 0, 0 }
};

void Utils::initApp(int argc, char *argv[]) {
    string commands;
    int port = DEFAULT_PORT;
    int c;

    // not print error for invalid option or missing option argument
    opterr = 0;

    while ((c = getopt_long(argc, argv, Utils::optstring.c_str(), Utils::long_options, NULL)) != -1) {
        switch (c) {
            case 'v':
                Utils::printVersion();
                return;

            case '?':
            case 'h':
                Utils::printHelp();
                return;

            case 'p':
                if (!(port = strtol(optarg, NULL, 10))) {
                    exitError(string(PACKAGE_STRING) + "\n\n" + "error: port '" + optarg + "' is invalid\n");
                }
                break;

            case 'i':
                commands += "i";
                break;
        }
    }

    if (!commands.length() || (optind != (argc - 1))) {
        Utils::printHelp();
    } else {
        string target = argv[optind];

        // connection to node
        Connection connection(target, port);

        Utils::runCommands(commands, connection);
    }
}

void Utils::printVersion(void) {
    cout << PACKAGE_NAME << " version " << PACKAGE_VERSION << " by reizu <" << PACKAGE_BUGREPORT << ">" << endl;
}

void Utils::printHelp(void) {
    cout << PACKAGE_STRING << endl;

    cout << endl << "usage: izubitcoin <commands>... [options]... <target>" << endl << endl;

    cout << "SCAN COMMANDS:" << endl;
    cout << "\t-i, --info\tprint bitcoin node version" << endl << endl;

    cout << "MISC COMMANDS:" << endl;
    cout << "\t-h, --help\tshow this help and exits" << endl;
    cout << "\t-v, --version\tshow version and exits" << endl << endl;

    cout << "OPTIONS:" << endl;
    cout << "\t-p <port>, --port <port>\tconnect to this port instead of default " << DEFAULT_PORT << endl << endl;

    cout << "TARGET:" << endl;
    cout << "\tIP/hostname of bitcoin node" << endl << endl;

    cout << "EXAMPLES:" << endl;
    cout << "\tizubitcoin -i 192.168.1.1" << endl;
    cout << "\tizubitcoin --info --port 8334 192.168.1.1" << endl;
}

void Utils::exitError(string msg) {
    cerr << msg << endl;
    exit(EXIT_FAILURE);
}

void Utils::runCommands(string commands, Connection &connection) {
    for (char c : commands) {
        switch (c) {
            case 'i':
                Info info(&connection);
                info.run();
                break;
        }
    }
}
