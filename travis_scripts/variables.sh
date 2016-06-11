#!/bin/bash

export DIR=~/patlms_bindir
export GTEST_INSTALL_DIR=${DIR}/gtest
export GMOCK_INSTALL_DIR=${DIR}/gmock
export PATLMS_INSTALL_DIR=${DIR}/libpatlms
export AGENT_INSTALL_DIR=${DIR}/agent
export SERVER_INSTALL_DIR=${DIR}/server

# libpatlms flags
export CPPFLAGS="$CPPFLAGS -I${PATLMS_INSTALL_DIR}/include"
export LDFLAGS="$LDFLAGS -L${PATLMS_INSTALL_DIR}/lib"
export LD_LIBRARY_PATH="${PATLMS_INSTALL_DIR}/lib"

