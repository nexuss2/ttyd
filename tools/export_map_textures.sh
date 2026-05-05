#!/usr/bin/env bash
set -e

if [ "$#" -eq 0 ]; then
  maps="aaa_00 nok_01 hei_00"
else
  maps="$@"
fi

for map in $maps; do
  echo "Exporting $map"

  ./build/pc/map_texture_export "$map"
  ./build/pc/map_info_export "$map"

  python3 tools/ppm_to_png.py "build/pc/${map}_*.ppm"
  python3 tools/png_contact_sheet.py "$map"
  ./tools/make_map_texture_report.sh "$map"

  echo "Wrote:"
  echo "  build/pc/${map}_texture_names.txt"
  echo "  build/pc/${map}_sheet.png"
done
