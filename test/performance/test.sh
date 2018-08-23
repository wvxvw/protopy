#!/usr/bin/env bash

set -xe

perfgen -s 234 -o ./234_test.proto -n 10 -b '234_%d.bin'
protoc -I. --python_out=. ./234_test.proto
perfrunner -n 1 -f ./234_test.proto -b ./234_0.bin -p ./234_test_pb2.py -e
# cat ./234_0.bin | protoc -I. --decode_raw
# cat ./234_0.bin | protoc -I. --decode=AllMessages ./234_test.proto 
