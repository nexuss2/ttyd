#!/usr/bin/env bash
set -e

mkdir -p build/pc

cc -DPLATFORM_PC -c src/platform/pc/file.c -o build/pc/file.o
cc -DPLATFORM_PC -c src/platform/pc/input.c -o build/pc/input.o
cc -DPLATFORM_PC -c src/platform/pc/time.c -o build/pc/time.o
cc -DPLATFORM_PC -c src/platform/pc/memory_pc.c -o build/pc/memory_pc.o
cc -DPLATFORM_PC -c src/platform/pc/dvdmgr_pc.c -o build/pc/dvdmgr_pc.o
cc -DPLATFORM_PC -c src/platform/pc/arcdrv_pc.c -o build/pc/arcdrv_pc.o
cc -DPLATFORM_PC -c src/platform/pc/fadedrv_pc.c -o build/pc/fadedrv_pc.o
cc -DPLATFORM_PC -c src/platform/pc/envdrv_pc.c -o build/pc/envdrv_pc.o
cc -DPLATFORM_PC -c src/platform/pc/windowdrv_pc.c -o build/pc/windowdrv_pc.o
cc -DPLATFORM_PC -c src/platform/pc/effdrv_pc.c -o build/pc/effdrv_pc.o

cc -DPLATFORM_PC \
  pc_main.c \
  build/pc/file.o \
  build/pc/input.o \
  build/pc/time.o \
  build/pc/memory_pc.o \
  build/pc/dvdmgr_pc.o \
  build/pc/arcdrv_pc.o \
  build/pc/fadedrv_pc.o \
  build/pc/envdrv_pc.o \
  build/pc/windowdrv_pc.o \
  build/pc/effdrv_pc.o \
  -o build/pc/ttyd_pc

cc -DPLATFORM_PC pc_asset_test.c build/pc/file.o -o build/pc/asset_test
cc -DPLATFORM_PC pc_arcdrv_test.c build/pc/file.o build/pc/arcdrv_pc.o -o build/pc/arcdrv_test

./build/pc/ttyd_pc
./build/pc/asset_test
./build/pc/arcdrv_test
