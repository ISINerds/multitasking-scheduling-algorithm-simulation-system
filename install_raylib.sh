#!/bin/bash


if [ ! -d "build_raylib" ]; then
    mkdir build_raylib
fi

cd build_raylib || exit

cmake ..

make