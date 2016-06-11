#!/bin/bash

set -e
set -v

travis_scripts/build_script.sh
travis_scripts/check_with_valgrind.sh

