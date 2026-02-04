#!/bin/bash
set -e

# Build if needed, then run
./build.sh "${1:-Debug}"
./build/bin/MyGame
