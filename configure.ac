AC_INIT([izubitcoin], [0.1], [reizu@seguridadoverflow.com])

AC_CONFIG_SRCDIR([src/izubitcoin.cpp])

AC_CONFIG_HEADERS([config.h])

AC_DEFINE([DEFAULT_PORT], [8333], [Default port for connect to a node])
AC_DEFINE([CONNECTION_TIMEOUT], [2], [Timeout (seconds) for connect to a node])
AC_DEFINE([SEND_MSG_TIMEOUT], [5], [Timeout (seconds) for send a message to a node])
AC_DEFINE([RECV_MSG_TIMEOUT], [5], [Timeout (seconds) for receive a message from a node])
AC_DEFINE([NODE_VERSION], [70015], [Node version sent])
AC_DEFINE([USER_AGENT], ["/izubitcoin/"], [User agent sent])

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([11], [ext], [mandatory])

AC_CHECK_LIB([ssl], [main], [], [AC_MSG_ERROR([libssl missing])])
AC_CHECK_LIB([crypto], [main], [], [AC_MSG_ERROR([libcrypto missing])])

AC_CONFIG_FILES([Makefile src/Makefile])

AC_PROG_INSTALL

AC_OUTPUT
