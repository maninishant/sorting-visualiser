# Sorting Visualizer

A C++ sorting algorithm visualizer built using SFML 3.1.0.

## Algorithms
- Bubble Sort
- Merge Sort

## Tech Stack
- C++
- SFML 3.1.0
- CMake

## Features
- Visual representation of sorting operations
- Algorithm selection
- Animated comparisons and swaps

## Requirements
- C++17 compatible compiler
- CMake
- SFML 3.1.0

## Build and Run (Windows)

1. Install SFML 3.1.0 with a MinGW package compatible with your compiler.
2. Configure the project, providing the path to SFML's CMake directory:

   ```powershell
   cmake -S . -B build -DSFML_DIR="C:/path/to/SFML-3.1.0/lib/cmake/SFML"