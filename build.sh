#!/bin/bash

CWD=$(pwd)

if [ ! -d $CWD/build ]; then
  mkdir -p $CWD/build;
  mkdir -p $CWD/bin;
  cd build
  cmake ..
  cd $CWD
fi

cd build
make
cp Asteroids ../bin/Asteroids