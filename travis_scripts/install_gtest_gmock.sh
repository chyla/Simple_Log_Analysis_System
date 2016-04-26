#!/bin/bash

set -e

export DIR=~/patlms_bindir

export GTEST_INSTALL_DIR=${DIR}/gtest
export GMOCK_INSTALL_DIR=${DIR}/gmock
export TMP_DIR=/tmp/gtest_gmock

# Set compiler
export CXX=$CXX_COMPILER
export CC=$CC_COMPILER

mkdir -p $TMP_DIR $GTEST_INSTALL_DIR $GMOCK_INSTALL_DIR
cd $TMP_DIR

git clone https://github.com/google/googletest

### install Google Test
cd $TMP_DIR/googletest/googletest
cmake .
make

mkdir -p $GTEST_INSTALL_DIR/include $GTEST_INSTALL_DIR/lib
cp -r include $GTEST_INSTALL_DIR
cp libgtest_main.a libgtest.a $GTEST_INSTALL_DIR/lib

### install Google Mock
cd $TMP_DIR/googletest/googlemock
cmake .
make

mkdir -p $GMOCK_INSTALL_DIR/include $GMOCK_INSTALL_DIR/lib
cp -r include $GMOCK_INSTALL_DIR
cp libgmock_main.a libgmock.a $GMOCK_INSTALL_DIR/lib

