#!/bin/bash

set -e

aclocal

autoreconf \
	--force \
	--install

