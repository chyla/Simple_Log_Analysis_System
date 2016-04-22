#!/bin/bash

set -e 

START_DIR=`pwd`

export DIR=~/patlms_bindir
export GTEST_INSTALL_DIR=${DIR}/gtest
export GMOCK_INSTALL_DIR=${DIR}/gmock
export PATLMS_INSTALL_DIR=${DIR}/libpatlms
export AGENT_INSTALL_DIR=${DIR}/agent
export SERVER_INSTALL_DIR=${DIR}/server

# Google Test & Google Mock flags
export LDFLAGS="$LDFLAGS -L${GTEST_INSTALL_DIR}/lib -L${GMOCK_INSTALL_DIR}/lib"
export CPPFLAGS="$CPPFLAGS -I${GTEST_INSTALL_DIR}/include -I${GMOCK_INSTALL_DIR}/include"


# build libpatlms
cd libpatlms
./configure --prefix=${PATLMS_INSTALL_DIR}
make
make install
make check

# libpatlms flags
export CPPFLAGS="$CPPFLAGS -I${PATLMS_INSTALL_DIR}/include"
export LDFLAGS="$LDFLAGS -L${PATLMS_INSTALL_DIR}/lib"
export LD_LIBRARY_PATH="${PATLMS_INSTALL_DIR}/lib"

# build server
cd ${START_DIR}/server
./configure --prefix=${SERVER_INSTALL_DIR}
make
make install
make check

# build agent
cd ${START_DIR}/agent
./configure --prefix=${AGENT_INSTALL_DIR}
make
make install
make check

