#!/usr/bin/env bash
set -e

mkdir -p build/pc

cc -DPLATFORM_PC -c src/platform/pc/file.c -o build/pc/file.o
cc -DPLATFORM_PC -c src/platform/pc/input.c -o build/pc/input.o
cc -DPLATFORM_PC -c src/platform/pc/time.c -o build/pc/time.o
