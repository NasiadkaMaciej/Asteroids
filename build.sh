#!/bin/bash

CWD=$(pwd)

if [ ! -d $CWD/build ]; then
  mkdir -p $CWD/build;
  cd build
  cmake ..
  cd $CWD
fi

cd build
make
cp Asteroids ../bin/Asteroids