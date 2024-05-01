#!/bin/bash

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
cd "$ENGINE_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Debug "$PROJECT_ROOT/HrothCore"
cmake --build . --config Debug
cp "$PROJECT_ROOT/build/HrothCore/Debug/HrothCore.lib" "$BIN_DEBUG_DIR"
echo "Engine debug build complete."

# Create sandbox build directory if it doesn't exist
mkdir -p "$SANDBOX_BUILD_DIR"

# Build sandbox (debug)
cd "$SANDBOX_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Debug "$PROJECT_ROOT/Sandbox"
cmake --build . --config Debug
cp "$PROJECT_ROOT/build/Sandbox/Debug/Sandbox.exe" "$BIN_DEBUG_DIR"
echo "Sandbox debug build complete."
