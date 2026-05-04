#!/usr/bin/env bash
set -e

mkdir -p build/pc

cc -DPLATFORM_PC -c src/platform/pc/file.c -o build/pc/file.o
cc -DPLATFORM_PC -c src/platform/pc/input.c -o build/pc/input.o
cc -DPLATFORM_PC -c src/platform/pc/time.c -o build/pc/time.o
cc -DPLATFORM_PC -c src/platform/pc/dvdmgr_pc.c -o build/pc/dvdmgr_pc.o
cc -DPLATFORM_PC -c src/platform/pc/memory_pc.c -o build/pc/memory_pc.o

cc -DPLATFORM_PC \
  pc_main.c \
  build/pc/file.o \
  build/pc/input.o \
  build/pc/time.o \
  build/pc/dvdmgr_pc.o \
  build/pc/memory_pc.o \
  -o build/pc/ttyd_pc

./build/pc/ttyd_pc
