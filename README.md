# izubitcoin

izubitcoin is a simple tool for interact with bitcoin nodes at protocol level.

### Prerequisites

Computer running GNU Linux (with dev tools installed).

### Installing

Compile:
```
$ ./autogen.sh
$ ./configure
$ make
```

Install (optional):

```
$ sudo make install
```

### Using

Run without arguments for help:

```
$ izubitcoin

izubitcoin 0.1

usage: izubitcoin <commands>... [options]... <target>

SCAN COMMANDS:
	-i, --info	print bitcoin node version

MISC COMMANDS:
	-h, --help	show this help and exits
	-v, --version	show version and exits

OPTIONS:
	-p <port>, --port <port>	connect to this port instead of default 8333

TARGET:
	IP/hostname of bitcoin node

EXAMPLES:
	izubitcoin -i 192.168.1.1
	izubitcoin --info --port 8334 192.168.1.1

```

Bitcoin node version example:

```
$ izubitcoin -i 101.132.26.28
Connected to 101.132.26.28:8333
Bitcoin node version: /Bitcoin ABC:0.16.1(EB8.0)/
```

### Contributing

https://github.com/segoverflow/izubitcoin

### Authors

* **reizu** - http://seguridadoverflow.com

### License

izubitcoin is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.
