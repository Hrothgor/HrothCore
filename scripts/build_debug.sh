#!/bin/bash

# Define color codes
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Determine the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Set project root directory
PROJECT_ROOT="$SCRIPT_DIR/.."

# Set engine build directory
ENGINE_BUILD_DIR="$PROJECT_ROOT/build/HrothCore"

# Set sandbox build directory
SANDBOX_BUILD_DIR="$PROJECT_ROOT/build/Sandbox"

# Set bin directory
BIN_DEBUG_DIR="$PROJECT_ROOT/bin/Debug"

# Create engine build directory if it doesn't exist
mkdir -p "$ENGINE_BUILD_DIR"
mkdir -p "$BIN_DEBUG_DIR"

# Build the engine project (debug)
echo -e "${YELLOW}\nEngine debug build Start.\n${NC}"
cd "$ENGINE_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Debug "$PROJECT_ROOT/HrothCore"
if ! cmake --build . --config Debug; then
    echo -e "${RED}\nFailed to build engine.${NC}\n"
    exit 1
fi
cp "$PROJECT_ROOT/build/HrothCore/Debug/HrothCore.lib" "$BIN_DEBUG_DIR"
echo -e "${GREEN}\nEngine debug build complete.\n${NC}"

# Create sandbox build directory if it doesn't exist
mkdir -p "$SANDBOX_BUILD_DIR"

# Build sandbox (debug)
echo -e "${YELLOW}\nSandbox debug build Start.\n${NC}"
cd "$SANDBOX_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Debug "$PROJECT_ROOT/Sandbox"
if ! cmake --build . --config Debug; then
    echo -e "${RED}\nFailed to build sandbox.${NC}\n"
    exit 1
fi
cp "$PROJECT_ROOT/build/Sandbox/Debug/Sandbox.exe" "$BIN_DEBUG_DIR"
echo -e "${GREEN}\nSandbox debug build complete.\n${NC}"
