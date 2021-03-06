#!/bin/bash

if [ ! -d include ]; then mkdir include ; fi
if [ ! -d lib ]; then mkdir lib; fi
T=$PWD

if [ ! -e lib/libuseful.a ]; then
    echo "building: libuseful.a"
    cd ngtemplate/lib/libuseful/src
    for f in *.c; do
        gcc -Os -Iinclude -c $f
    done
    ar cr libuseful.a *.o
    cp include/*.h $T/include
    mv libuseful.a $T/lib
    rm *.o
    cd $T
fi

if [ ! -e lib/libngtemplate.a ]; then
    echo "building: libngtemplate.a"
    cd ngtemplate/src
    for f in *.c; do
        gcc -Os -Iinclude -I../../include -c $f
    done
    ar cr libngtemplate.a *.o
    rm *.o
    cp include/*.h $T/include
    mv libngtemplate.a $T/lib
    cd $T
fi

ld -lzzip 2>/dev/null
if [ $? -eq 1 ]; then
    echo "building: libzzip"
    wget -O zziplib-0.13.58.tar.bz2 http://sourceforge.net/projects/zziplib/files/zziplib13/0.13.58/zziplib-0.13.58.tar.bz2/download
    tar jxf zziplib-0.13.58.tar.bz2
    D=zziplib-0.13.58
    (cd $D && ./configure && make) >/dev/null 2>&1
    mkdir include/zzip
    cp $D/zzip/zzip.h include
    cp $D/zzip/*.h include/zzip
    cp $D/Linux_*/zzip/_config.h include/zzip
    cp $D/Linux_*/zzip/.libs/libzzip.a lib

    rm -rf $D zziplib-*.tar.bz2
fi
rm -f a.out
