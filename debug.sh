#!/bin/sh
mkdir -p build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ../ledify
cmake --build .
sudo ln -f -s $(pwd)/src/ledify /usr/local/bin/ledify
