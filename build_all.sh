#!/bin/bash
set -e

echo "=== Configuring ==="
cmake -B build

echo "=== Building ==="
cmake --build build

echo "=== Running Tests ==="
cd build && ctest --output-on-failure

echo "=== Done ==="
echo "Executables available at:"
echo "  ./build/bin/basic_game"
echo "  ./build/bin/pong"
