// Copyright (c) 2018 reizu
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/MIT.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "options.h"

using namespace std;

Options::Options() : core(DEFAULT_CORE), testnet(DEFAULT_TESTNET) {}

Options::Options(bool core, bool testnet) {
    this->core = core;
    this->testnet = testnet;
}

bool Options::isCore() {
    return core;
}

bool Options::isTestnet() {
    return testnet;
}

void Options::setCore(bool core) {
    this->core = core;
}

void Options::setTestnet(bool testnet) {
    this->testnet = testnet;
}
