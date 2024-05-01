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
BIN_RELEASE_DIR="$PROJECT_ROOT/bin/Release"

# Create engine build directory if it doesn't exist
mkdir -p "$ENGINE_BUILD_DIR"
mkdir -p "$BIN_RELEASE_DIR"

# Build the engine project (release)
echo -e "${YELLOW}\nEngine release build Start.\n${NC}"
cd "$ENGINE_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Release "$PROJECT_ROOT/HrothCore"
if ! cmake --build . --config Release; then
    echo -e "${RED}\nFailed to build engine.${NC}\n"
    exit 1
fi
cp "$PROJECT_ROOT/build/HrothCore/Release/HrothCore.lib" "$BIN_RELEASE_DIR"
echo -e "${GREEN}\nEngine release build complete.\n${NC}"

# Create sandbox build directory if it doesn't exist
mkdir -p "$SANDBOX_BUILD_DIR"

# Build sandbox (release)
echo -e "${YELLOW}\nSandbox release build Start.\n${NC}"
cd "$SANDBOX_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Release "$PROJECT_ROOT/Sandbox"
if ! cmake --build . --config Release; then
    echo -e "${RED}\nFailed to build sandbox.${NC}\n"
    exit 1
fi
cp "$PROJECT_ROOT/build/Sandbox/Release/Sandbox.exe" "$BIN_RELEASE_DIR"
echo -e "${GREEN}\nSandbox release build complete.\n${NC}"
