#!/bin/bash
set -e

BUILD_DIR="build"

# Build first (includes test target)
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
ninja smithy_tests

echo ""
echo "Running tests..."
echo ""

# Run tests with verbose output
# Use --success to show passing tests too, or remove for failures only
./bin/smithy_tests "$@"
