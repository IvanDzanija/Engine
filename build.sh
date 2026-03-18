#!/usr/bin/env bash
set -euo pipefail

DEBUG_BUILD_DIR="build-debug"
RELEASE_BUILD_DIR="build-release"
JOBS="$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)"

echo "==> Syncing submodules..."
git submodule sync --recursive
git submodule update --init --recursive

echo "==> Configuring Debug build with sanitizers..."
cmake -S . -B "$DEBUG_BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DENABLE_SANITIZERS=ON

echo "==> Building Debug..."
cmake --build "$DEBUG_BUILD_DIR" -j"$JOBS"

# echo "==> Configuring Release build..."
# cmake -S . -B "$RELEASE_BUILD_DIR" \
#     -DCMAKE_BUILD_TYPE=Release \
#     -DENABLE_SANITIZERS=OFF
#
# echo "==> Building Release..."
# cmake --build "$RELEASE_BUILD_DIR" -j"$JOBS"

echo "==> Build finished."
# echo "Debug executables are in:   $DEBUG_BUILD_DIR/<target_name>/"
# echo "Release executables are in: $RELEASE_BUILD_DIR/<target_name>/"
