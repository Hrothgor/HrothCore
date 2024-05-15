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
cmake -DCMAKE_BUILD_TYPE=Debug "$PROJECT_ROOT/Sandbox"
if ! cmake --build . --config Debug -j4; then
    echo -e "${RED}\nFailed to build.${NC}\n"
    exit 1
fi
# Create bin directory if it doesn't exist
mkdir -p "$BIN_DIR"
# Copy binary to bin directory
if [ "$(uname)" == "Darwin" ]; then
    cp "$PROJECT_ROOT/build/Sandbox" "$BIN_DIR"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    cp "$PROJECT_ROOT/build/Sandbox" "$BIN_DIR"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    cp "$PROJECT_ROOT/build/Debug/Sandbox.exe" "$BIN_DIR"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    cp "$PROJECT_ROOT/build/Debug/Sandbox.exe" "$BIN_DIR"
fi
echo -e "${GREEN}\nDebug build complete.\n${NC}"