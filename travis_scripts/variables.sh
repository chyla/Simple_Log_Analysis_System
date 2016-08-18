#!/bin/bash

export DIR=~/slas_bindir
export GTEST_INSTALL_DIR=${DIR}/gtest
export GMOCK_INSTALL_DIR=${DIR}/gmock
export SLAS_INSTALL_DIR=${DIR}/libpatlms
export AGENT_INSTALL_DIR=${DIR}/agent
export SERVER_INSTALL_DIR=${DIR}/server
export BASH_INSTALL_DIR=${DIR}/bash

# libpatlms flags
export CPPFLAGS="$CPPFLAGS -I${SLAS_INSTALL_DIR}/include"
export LDFLAGS="$LDFLAGS -L${SLAS_INSTALL_DIR}/lib"
export LD_LIBRARY_PATH="${SLAS_INSTALL_DIR}/lib"

