#!/bin/sh
mkdir -p build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ../ledify
make