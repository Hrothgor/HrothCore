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
BIN_DIR="$PROJECT_ROOT/bin/Release"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Build the project (release)
echo -e "${YELLOW}\nRelease build Start.\n${NC}"
cd "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Release "$PROJECT_ROOT/Sandbox" 2>&1
if ! cmake --build . --config Release 2>&1; then
    echo -e "${RED}\nFailed to build.${NC}\n"
    exit 1
fi
# Create bin directory if it doesn't exist
mkdir -p "$BIN_DIR"
# Copy binary to bin directory
cp "$PROJECT_ROOT/build/Release/Sandbox.exe" "$BIN_DIR"
echo -e "${GREEN}\nRelease build complete.\n${NC}"