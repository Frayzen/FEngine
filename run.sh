#!/bin/sh

if [ ! -d ./build/ ]; then
  cmake . -B build
fi
make -C build -j
if [ $? -eq 0 ]; then
  ./build/engine
fi
