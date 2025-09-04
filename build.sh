#!/bin/bash

# Build script for 6 nimmt! Contest Framework

echo "Building 6 nimmt! Contest Framework..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
echo "Building..."
make -j$(nproc)

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo ""
    echo "Available executables:"
    echo "  ./sixnimmt_contest  - Main contest program"
    echo "  ./test_game         - Simple test program"
    echo ""
    echo "Run with: ./sixnimmt_contest"
else
    echo "Build failed!"
    exit 1
fi
