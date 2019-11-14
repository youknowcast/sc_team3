#!/bin/sh
set -x

sudo yum install git gcc-c++

wget https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/slibc/slibc-0.9.2.tar.gz
tar xfvz slibc-0.9.2.tar.gz
cd slibc-0.9.2
make

cd ..
cp -a test_all.c test_all.c_bak
sed -i -e 's/IS_NOT_SLIBC/IS_SLIBC/g' test_all.c

export LD_LIBRARY_PATH=./slibc-0.9.2/src/
gcc -L./slibc-0.9.2/src test_all.c -lslibc -o with_slibc

mv test_all.c_bak test_all.c

./with_slibc


