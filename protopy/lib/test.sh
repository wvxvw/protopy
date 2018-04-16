#!/usr/bin/env bash

set -xeo pipefail

echo "test: 123" | protoc --encode=Test -IPATH=. test.proto > ./test.bin
./protopy $(< ./test.bin)
