#!/bin/sh
mkdir -p build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ../ledify
make
sudo ln -f -s $(pwd)/src/ledify_rpi /usr/local/bin/ledify
