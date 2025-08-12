#!/bin/bash

set -e

echo "Creating bin directory..."
mkdir -p bin

g++ -I./src -O3 -c src/main.cpp -o bin/main.o
g++ -I./src -O3 -c src/mandelbrot.cpp -o bin/mandelbrot.o

nvcc -I./src -O3 -c src/mandelbrot_cuda.cu -o bin/mandelbrot_cuda.o

echo "Linking all object files..."
nvcc -O3 -o bin/mandelbrot bin/main.o bin/mandelbrot.o bin/mandelbrot_cuda.o
rm bin/*.o

echo "--- Compilation successful! ---"+