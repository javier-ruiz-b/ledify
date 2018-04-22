#!/bin/sh
mkdir -p build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ../ledify
make
sudo ln -f -s $(pwd)/src/ledify_rpi /usr/local/bin/ledify
