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

if [ -e lib/libzzip.a ]; then exit 0; fi

mkdir include/zzip

if [ ! -e zziplib-0.13.58.tar.bz2 ]; then
    wget http://sourceforge.net/projects/zziplib/files/zziplib13/0.13.58/zziplib-0.13.58.tar.bz2/download
fi

if [ ! -d zziplib-0.13.58 ]; then
    tar jxf zziplib-0.13.58.tar.bz2 
fi

D=zziplib-0.13.58 

(cd $D && ./configure && make)

cp $D/zzip/zzip.h include
cp $D/zzip/types.h $D/zzip/conf.h include/zzip
cp $D/Linux_*/zzip/_config.h include/zzip
cp $D/Linux_*/zzip/.libs/libzzip.a lib

rm -rf $D
