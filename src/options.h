// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#ifndef _IZUBITCOIN_OPTIONS_H
#define _IZUBITCOIN_OPTIONS_H 1

using namespace std;

class Options {
    public:
        Options();
        Options(bool core, bool testnet);
        bool isCore();
        bool isTestnet();
        void setCore(bool core);
        void setTestnet(bool testnet);

    protected:
        bool core;
        bool testnet;
};

#endif
