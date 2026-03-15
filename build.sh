#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
BUILD_TYPE="${1:-Release}"

echo "==> Syncing submodules..."
git submodule sync --recursive
git submodule update --init --recursive

echo "==> Configuring project ($BUILD_TYPE)..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

echo "==> Building project..."
cmake --build "$BUILD_DIR" -j"$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)"

echo "==> Build finished."
echo "Executables are in: $BUILD_DIR/<target_name>/"
