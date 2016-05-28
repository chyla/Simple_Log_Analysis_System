#!/bin/bash

set -e 

function insert_module_name () {
  sed -r "s~(.*)~`echo -e '\e[36;1m'`$1`echo -e '\e[0m'`: \1~"
}

function build () {
  ./autogen.sh 
  ./configure --prefix=${PATLMS_INSTALL_DIR} 
  make
  make install
  make check
}

START_DIR=`pwd`

export DIR=~/patlms_bindir
export GTEST_INSTALL_DIR=${DIR}/gtest
export GMOCK_INSTALL_DIR=${DIR}/gmock
export PATLMS_INSTALL_DIR=${DIR}/libpatlms
export AGENT_INSTALL_DIR=${DIR}/agent
export SERVER_INSTALL_DIR=${DIR}/server

# Set compiler
export CXX=$CXX_COMPILER
export CC=$CC_COMPILER

# Google Test & Google Mock flags
export LDFLAGS="$LDFLAGS -L${GTEST_INSTALL_DIR}/lib -L${GMOCK_INSTALL_DIR}/lib"
export CPPFLAGS="$CPPFLAGS -I${GTEST_INSTALL_DIR}/include -I${GMOCK_INSTALL_DIR}/include"


# build libpatlms
cd ${START_DIR}/libpatlms
build | insert_module_name libpatlms

# libpatlms flags
export CPPFLAGS="$CPPFLAGS -I${PATLMS_INSTALL_DIR}/include"
export LDFLAGS="$LDFLAGS -L${PATLMS_INSTALL_DIR}/lib"
export LD_LIBRARY_PATH="${PATLMS_INSTALL_DIR}/lib"

# build server
cd ${START_DIR}/server
build | insert_module_name server

# build agent
cd ${START_DIR}/agent
build | insert_module_name agent

# check web server
cd ${START_DIR}/web
tox | insert_module_name web

