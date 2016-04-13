#!/bin/bash

set -e

export GTEST_INSTALL_DIR=/usr/local/gtest
export GMOCK_INSTALL_DIR=/usr/local/gmock
export TMP_DIR=/tmp/gtest_gmock

mkdir -p $TMP_DIR
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

### set environment variables in .bashrc
cat <<EOF >> /home/vagrant/.bashrc

# Google Test and Google Mock flags
export LDFLAGS="$LDFLAGS -L${GTEST_INSTALL_DIR}/lib -L${GMOCK_INSTALL_DIR}/lib"
export CPPFLAGS="$CPPFLAGS -I${GTEST_INSTALL_DIR}/include -I${GMOCK_INSTALL_DIR}/include"
EOF

