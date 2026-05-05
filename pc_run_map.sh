#!/usr/bin/env bash
set -e

MAP="${1:-aaa_00}"
MODE="${2:-auto}"

./build/pc/sdl_real_map_runner "$MAP" "$MODE"

echo "Wrote build/pc/sdl_real_map_runner.bmp"
