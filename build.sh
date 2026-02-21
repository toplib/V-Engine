#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_DIR/build"

mkdir -p "$BUILD_DIR"

cd "$BUILD_DIR"
cmake ..
cmake --build .

