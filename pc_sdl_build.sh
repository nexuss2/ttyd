#!/usr/bin/env bash
set -e

mkdir -p build/pc

cc -DPLATFORM_PC -c src/platform/pc/file.c -o build/pc/file.o
cc -DPLATFORM_PC -c src/platform/pc/arcdrv_pc.c -o build/pc/arcdrv_pc.o
cc -DPLATFORM_PC -c src/platform/pc/mapdrv_pc.c -o build/pc/mapdrv_pc.o

cc -DPLATFORM_PC -Iinclude -c src/platform/pc/render_sdl_pc.c -o build/pc/render_sdl_pc.o $(pkg-config --cflags sdl2)
cc -DPLATFORM_PC -Iinclude -c src/platform/pc/tpl_export_pc.c -o build/pc/tpl_export_pc.o
cc -DPLATFORM_PC -Iinclude -c src/platform/pc/texture_runtime_sdl.c -o build/pc/texture_runtime_sdl.o
cc -DPLATFORM_PC -Iinclude -c src/platform/pc/map_texture_runtime_sdl.c -o build/pc/map_texture_runtime_sdl.o

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_window_test.c \
  build/pc/render_sdl_pc.o \
  -o build/pc/sdl_window_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_rgb_test.c \
  build/pc/render_sdl_pc.o \
  -o build/pc/sdl_rgb_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_cmpr_direct_test.c \
  build/pc/file.o \
  build/pc/arcdrv_pc.o \
  build/pc/tpl_export_pc.o \
  build/pc/render_sdl_pc.o \
  -o build/pc/sdl_cmpr_direct_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_cmpr_sheet_test.c \
  build/pc/file.o \
  build/pc/arcdrv_pc.o \
  build/pc/tpl_export_pc.o \
  build/pc/render_sdl_pc.o \
  -o build/pc/sdl_cmpr_sheet_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_texture_cache_test.c \
  build/pc/file.o \
  build/pc/arcdrv_pc.o \
  build/pc/tpl_export_pc.o \
  build/pc/render_sdl_pc.o \
  -o build/pc/sdl_texture_cache_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_texture_runtime_test.c \
  build/pc/file.o \
  build/pc/arcdrv_pc.o \
  build/pc/tpl_export_pc.o \
  build/pc/render_sdl_pc.o \
  build/pc/texture_runtime_sdl.o \
  -o build/pc/sdl_texture_runtime_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_runtime_texture_sheet_test.c \
  build/pc/file.o \
  build/pc/arcdrv_pc.o \
  build/pc/tpl_export_pc.o \
  build/pc/render_sdl_pc.o \
  build/pc/texture_runtime_sdl.o \
  -o build/pc/sdl_runtime_texture_sheet_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_map_texture_set_test.c \
  build/pc/file.o \
  build/pc/arcdrv_pc.o \
  build/pc/mapdrv_pc.o \
  build/pc/tpl_export_pc.o \
  build/pc/render_sdl_pc.o \
  build/pc/texture_runtime_sdl.o \
  build/pc/map_texture_runtime_sdl.o \
  -o build/pc/sdl_map_texture_set_test \
  $(pkg-config --libs sdl2)

cc -DPLATFORM_PC -Iinclude \
  pc_sdl_named_texture_test.c \
  build/pc/file.o \
  build/pc/arcdrv_pc.o \
  build/pc/mapdrv_pc.o \
  build/pc/tpl_export_pc.o \
  build/pc/render_sdl_pc.o \
  build/pc/texture_runtime_sdl.o \
  build/pc/map_texture_runtime_sdl.o \
  -o build/pc/sdl_named_texture_test \
  $(pkg-config --libs sdl2)

./build/pc/sdl_window_test
./build/pc/sdl_rgb_test
./build/pc/sdl_cmpr_direct_test
./build/pc/sdl_cmpr_sheet_test
./build/pc/sdl_texture_cache_test
./build/pc/sdl_texture_runtime_test
./build/pc/sdl_runtime_texture_sheet_test
./build/pc/sdl_map_texture_set_test aaa_00
./build/pc/sdl_named_texture_test
