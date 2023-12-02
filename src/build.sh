#!/bin/sh
set -xe

RAYLIB_PATH="../build_raylib/_deps/raylib-build/raylib"

gcc -I"$RAYLIB_PATH/include" -L"$RAYLIB_PATH" -o main2 main2.c -lraylib -lm -lglfw -ldl -lpthread
