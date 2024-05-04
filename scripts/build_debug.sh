#!/bin/bash

# Define color codes
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Determine the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

PROJECT_ROOT="$SCRIPT_DIR/.."
BUILD_DIR="$PROJECT_ROOT/build"
BIN_DIR="$PROJECT_ROOT/bin/Debug"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Build the project (debug)
echo -e "${YELLOW}\nDebug build Start.\n${NC}"
cd "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Debug "$PROJECT_ROOT/Sandbox" 2>&1
if ! cmake --build . --config Debug 2>&1; then
    echo -e "${RED}\nFailed to build.${NC}\n"
    exit 1
fi
# Create bin directory if it doesn't exist
mkdir -p "$BIN_DIR"
# Copy binary to bin directory
cp "$PROJECT_ROOT/build/Debug/Sandbox.exe" "$BIN_DIR"
echo -e "${GREEN}\nDebug build complete.\n${NC}"