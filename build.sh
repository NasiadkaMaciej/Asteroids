#!/bin/bash

CWD=$(pwd)

if [ ! -d $CWD/build ]; then
  mkdir $CWD/build
  cd build
  cmake ..
fi

cd $CWD/build
make
cp Asteroids ../bin/Asteroids