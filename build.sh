#!/bin/bash

CWD=$(pwd)

if [ ! -d $CWD/build ]; then
  mkdir $CWD/build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Debug ..
fi

cd $CWD/build
make -j4
cp Asteroids ../bin/Asteroids