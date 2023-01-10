#!/bin/bash
THREADS=$(($(getconf _NPROCESSORS_ONLN 2> /dev/null)-2))
VERSION=12
HOST=x86_64-pc-linux-gnu
readonly THREADS
readonly VERSION
readonly HOST
sudo apt-get install -y build-essential flex
git clone git://gcc.gnu.org/git/gcc.git GccLatest
cd GccLatest
git checkout remotes/origin/releases/gcc-$VERSION
./contrib/download_prerequisites
cd ..
mkdir build
cd build
./../GccLatest/configure --prefix=$PWD/../TOOLCHAIN/GCC-$VERSION --enable-languages=c,c++ --host=$HOST --enable-vtable-verify --disable-multilib
make -j$THREADS
make install
cd ..
rm -rf ./GccLatest
rm -rf ./build
exit 0
