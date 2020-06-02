#!/bin/bash
export IBM_DB_HOME=/usr
export PHP_HOME=/usr/local/zendphp7
export PASE_TOOLS_HOME=/QOpenSys/usr
export AIX_TOOLS_HOME=/usr/local
export PATH=$PHP_HOME/bin:$PASE_TOOLS_HOME/bin:$AIX_TOOLS_HOME/bin:$PATH
export LIBPATH=$PHP_HOME/lib:$PASE_TOOLS_HOME/lib:$AIX_TOOLS_HOME/lib
export CC=gcc
export CFLAGS="-g -w -DPASE -I=.:$PHP_HOME/php/include"
export CCHOST=powerpc-ibm-aix6.1.0.0
export DB2PATH=/QOpenSys/usr
export DB2DIR=/QOpenSys/usr
# phpize
# ./configure --build=$CCHOST --host=$CCHOST --target=$CCHOST
make
make install
cp /usr/local/zendphp7/lib/php/20160303/* /usr/local/zendphp7/lib/php_extensions/.
