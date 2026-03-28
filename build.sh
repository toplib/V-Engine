#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_DIR/build"

BUILD=false
RUN=false

while getopts "br" opt; do
  case $opt in
    b) BUILD=true ;;
    r) RUN=true ;;
    *) echo "Usage: $0 [-b] [-r]" ; exit 1 ;;
  esac
done

if [ "$BUILD" = true ]; then
  mkdir -p "$BUILD_DIR"
  cd "$BUILD_DIR"
  cmake ..
  cmake --build .
fi

if [ "$RUN" = true ]; then
  cd "$BUILD_DIR"
  ./bin/V-Engine
fi
