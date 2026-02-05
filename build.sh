#!/bin/bash
set -e

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -G Ninja -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
ninja

echo ""
echo "Build complete: $BUILD_DIR/bin/basic_game"
