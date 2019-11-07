#!/bin/sh
set -x

wget https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/slibc/slibc-0.9.2.tar.gz
tar xfvz slibc-0.9.2.tar.gz
cd slibc-0.9.2
make

cd ..

export LD_LIBRARY_PATH=./slibc-0.9.2/src/
gcc -l./slibc -L./ test_all.c -lslibc

./a.out


