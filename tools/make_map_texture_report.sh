#!/usr/bin/env bash
set -e

map="$1"

if [ -z "$map" ]; then
  echo "usage: $0 map_name"
  exit 1
fi

names="build/pc/${map}_texture_names.txt"
sheet="build/pc/${map}_sheet.png"
report="build/pc/${map}_texture_report.md"

if [ ! -f "$names" ]; then
  echo "missing $names"
  exit 1
fi

cat > "$report" <<REPORT
# Texture Export Report: ${map}

## Contact Sheet

${sheet}

## Texture Names

\`\`\`
$(cat "$names")
\`\`\`
REPORT

echo "wrote $report"
