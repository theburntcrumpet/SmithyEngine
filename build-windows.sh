#!/bin/bash
set -e

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="build-win"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw64.cmake \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    ..
ninja

# Copy SDL2.dll if it exists and hasn't been copied
if [ -f /usr/x86_64-w64-mingw32/bin/SDL2.dll ] && [ ! -f bin/SDL2.dll ]; then
    cp /usr/x86_64-w64-mingw32/bin/SDL2.dll bin/
fi

# Copy SDL2_image.dll if it exists and hasn't been copied
if [ -f /usr/x86_64-w64-mingw32/bin/SDL2_image.dll ] && [ ! -f bin/SDL2_image.dll ]; then
    cp /usr/x86_64-w64-mingw32/bin/SDL2_image.dll bin/
fi

echo ""
echo "Build complete: $BUILD_DIR/bin/basic_game.exe"
