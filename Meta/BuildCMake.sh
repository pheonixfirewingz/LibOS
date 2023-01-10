#!/bin/bash
THREADS=$(($(getconf _NPROCESSORS_ONLN 2> /dev/null)-2))
VERSION=3.25.0
readonly VERSION
readonly THREADS
sudo apt-get install libssl-dev -y
cd TOOLCHAIN
wget https://github.com/Kitware/CMake/releases/download/v$VERSION/cmake-$VERSION.tar.gz
tar -xf cmake-$VERSION.tar.gz
rm -f cmake-$VERSION.tar.gz
cd cmake-$VERSION
./bootstrap --no-qt-gui --prefix=./../CMAKE-$VERSION --no-system-libs --parallel=$THREADS -- -DCMAKE_BUILD_TYPE:STRING=Release
make
make install
cd ..
rm -rf cmake-$VERSION
cd ..
exit 0