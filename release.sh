#!/bin/sh
mkdir -p build-release
cd build-release
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_C_COMPILER=/usr/bin/clang ../ledify
cmake --build .
sudo ln -f -s $(pwd)/src/ledify /usr/local/bin/ledify
