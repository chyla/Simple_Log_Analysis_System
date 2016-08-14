#!/bin/bash

set -e

aclocal

libtoolize \
	--force \
	--copy

autoreconf \
	--force \
	--install

