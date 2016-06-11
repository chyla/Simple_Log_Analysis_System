#!/bin/bash

set -e 
set -v

START_DIR=`pwd`

. travis_scripts/variables.sh

# Set compiler
export CXX=$CXX_COMPILER
export CC=$CC_COMPILER

# Google Test & Google Mock flags
export LDFLAGS="$LDFLAGS -L${GTEST_INSTALL_DIR}/lib -L${GMOCK_INSTALL_DIR}/lib"
export CPPFLAGS="$CPPFLAGS -I${GTEST_INSTALL_DIR}/include -I${GMOCK_INSTALL_DIR}/include"


# build libpatlms
cd libpatlms
./autogen.sh
./configure --prefix=${PATLMS_INSTALL_DIR}
make -j 3
make install
make -j 3 check

# build server
cd ${START_DIR}/server
./autogen.sh
./configure --prefix=${SERVER_INSTALL_DIR}
make -j 3
make install
make -j 3 check

sed -i 's~<!-- <apparmor mode="disabled"/> -->~<apparmor mode="disabled"/>~' ${SERVER_INSTALL_DIR}/etc/patlms/dbus.config.template

mkdir -p ${SERVER_INSTALL_DIR}/var/lib/patlms
mkdir -p ${SERVER_INSTALL_DIR}/var/log/patlms
mkdir -p ${SERVER_INSTALL_DIR}/var/run/patlms

# build agent
cd ${START_DIR}/agent
./autogen.sh
./configure --prefix=${AGENT_INSTALL_DIR}
make -j 3
make install
make -j 3 check

mkdir -p ${AGENT_INSTALL_DIR}/var/lib/patlms
mkdir -p ${AGENT_INSTALL_DIR}/var/log/patlms
mkdir -p ${AGENT_INSTALL_DIR}/var/run/patlms

# check web server
cd ${START_DIR}/web
tox

