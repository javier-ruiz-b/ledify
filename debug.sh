#!/bin/sh
mkdir -p build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ../ledify
ln -s $(pwd)/src/ledify_rpi /usr/local/bin/ledify
