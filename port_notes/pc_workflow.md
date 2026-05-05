# PC Port Workflow

## Quick health check

Run:

    ./pc_build.sh --quick

Builds the PC runner and runs the runtime health check only.

## Normal smoke test

Run:

    ./pc_build.sh

Builds and runs the normal PC smoke tests without bulk texture exporters.

## Export smoke test

Run:

    ./pc_build.sh --exports

Runs the normal smoke tests plus texture export tests.

## Map texture export

Run:

    ./tools/export_map_textures.sh aaa_00 nok_01 hei_00

Creates:
- map texture PPM/PNG files
- contact sheets
- texture name tables
- map texture reports

## Message UTF-8 export

Run:

    python3 tools/export_msg_utf8.py hei_00

Converts Shift-JIS message files into readable UTF-8 text in build/pc.

## Rules

- Keep orig/G8MJ01 clean.
- Do not rename or edit extracted game assets.
- Use separate branches for language patches or ROM hacks.
- Keep the original ninja decomp build passing.
