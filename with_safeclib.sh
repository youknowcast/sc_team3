#!/bin/sh
set -x

#sudo yum install -y git automake libtool

#cd ~/tmp
#git clone https://github.com/rurban/safeclib
#cd safeclib
#git checkout e939e2ae4958280717add4ea3c1db617c3bf81e7
#./build-aux/autogen.sh
#./configure
#make
#cd ..

cp -a test_all.c test_all.c_bak
sed -i -e 's/IS_NOT_SAFECLIB/IS_SAFECLIB/' test_all.c
gcc ./safeclib/src/.libs/libsafec-3.5.a test_all.c -o with_safeclib
mv test_all.c_bak test_all.c

./with_safeclib

