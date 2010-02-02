#!/bin/bash

mkdir include lib
T=$PWD
cd ngtemplate/lib/libuseful/src
cmake CMakeLists.txt
make
cp include/*.h $T/include
cp libuseful.a $T/lib
make clean
rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile
cd $T

cd ngtemplate/src
cmake CMakeLists.txt
make
cp include/*.h $T/include
cp libngtemplate.a $T/lib
make clean
rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile
cd $T
