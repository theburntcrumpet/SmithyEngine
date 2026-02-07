#!/bin/bash
set -e

./build.sh "${1:-Debug}"
cd build/bin
./audio_test
cd ..