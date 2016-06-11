#!/bin/bash

set -e
set -v

. travis_scripts/variables.sh

$SERVER_INSTALL_DIR/libexec/patlms/run_dbus &

sleep 1

valgrind --tool=memcheck --leak-check=full --track-origins=yes --num-callers=20 $SERVER_INSTALL_DIR/bin/patlms-server --nodaemon &

PID=$!
sleep 10
kill $PID


valgrind --tool=memcheck --leak-check=full --track-origins=yes --num-callers=20 $AGENT_INSTALL_DIR/bin/patlms-agent --nodaemon &

PID=$!
sleep 10
kill $PID

