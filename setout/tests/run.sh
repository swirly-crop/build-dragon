#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
make
./bin/c_app
./bin/cxx_app
./bin/asm_test
