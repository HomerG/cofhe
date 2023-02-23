#!/bin/bash

# files
headers=(cofhe cofhe_const communication conversion integer io \
    math number polynomial ringint spi support termite \
    termios termios2 uart vector_polynomial)

templates=(polynomial)

cpps=(cofhe conversion io integer math ringint \
    spi support termios termios2 termite uart)

lib=libcofhe

# compiler, archiver, and flags
cc=g++
ar=ar
aflags="rcs"
sflags="-O2 -std=c++17 -c"
dflags="-O2 -std=c++17 -c -fPIC"
lflags="-shared"

# directories
fdir=files
hdir=headers
sdir=static
ddir=shared

# functions
createDirectory()
{
    if [ ! -d "$@" ]; then
        mkdir $@
    fi
}

# create directories for headers, static, and dynamic libs
createDirectory $hdir
createDirectory $sdir
createDirectory $ddir

# copy headers
cp $fdir/*.h $hdir
cp $fdir/*.hpp $hdir

# compile for static lib
for cpp in "${cpps[@]}"
do
    $cc $sflags $fdir/$cpp.cpp -o $sdir/$cpp.o
done

# create static library
$ar $aflags $sdir/$lib.a $sdir/*.o

# compile for dynamic lib
for cpp in "${cpps[@]}"
do
    $cc $dflags $fdir/$cpp.cpp -o $ddir/$cpp.o
done

# create dynamic/shared library
$cc $lflags $ddir/*.o -o $ddir/$lib.so

# clean static object files
rm $sdir/*.o $ddir/*.o
