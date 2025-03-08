#!/bin/bash

set -e

if [ ! -d "build" ]; then
  echo "Creating build directory..."
  mkdir build
fi

cd build
make clean
cmake ..
make

./drawly
