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
BIN_RELEASE_DIR="$PROJECT_ROOT/bin/Release"

# Create engine build directory if it doesn't exist
mkdir -p "$ENGINE_BUILD_DIR"
mkdir -p "$BIN_RELEASE_DIR"

# Build the engine project (release)
cd "$ENGINE_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Release "$PROJECT_ROOT/HrothCore"
cmake --build . --config Release
cp "$PROJECT_ROOT/build/HrothCore/Release/HrothCore.lib" "$BIN_RELEASE_DIR"
echo "Engine release build complete."

# Create sandbox build directory if it doesn't exist
mkdir -p "$SANDBOX_BUILD_DIR"

# Build sandbox (release)
cd "$SANDBOX_BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Release "$PROJECT_ROOT/Sandbox"
cmake --build . --config Release
cp "$PROJECT_ROOT/build/Sandbox/Release/Sandbox.exe" "$BIN_RELEASE_DIR"
echo "Sandbox release build complete."
