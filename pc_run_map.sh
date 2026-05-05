#!/usr/bin/env bash
set -e

MAP="${1:-aaa_00}"
MODE="${2:-auto}"
EXTRA="${3:-}"

if [ -n "$EXTRA" ]; then
  ./build/pc/sdl_real_map_runner "$MAP" "$MODE" "$EXTRA"
else
  ./build/pc/sdl_real_map_runner "$MAP" "$MODE"
fi

echo "Wrote build/pc/sdl_real_map_runner.bmp"
