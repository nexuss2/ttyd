# PC Port Progress

Working native PC runner:
- pc_main.c
- pc_build.sh

Working PC platform shims:
- file backend
- input stub
- timing stub
- memory shim
- DVD manager shim
- arcOpen remap shim

Working asset loading:
- fade.tpl
- env.tpl
- font/msgWindow.tpl
- effect.tpl
- rel/aaa.rel

Working map loading:
- ./m/aaa_00/d
- ./m/aaa_00/t
- ./m/aaa_00/s
- ./m/aaa_00/c

Working map parsing:
- MapFileHeader
- chunk table
- named chunks
- information summary
- material_name_table preview
- texture_table preview

Current approach:
- Keep normal GameCube ninja build working
- Build native PC shims separately
- Avoid directly compiling Dolphin SDK headers with native cc
- Move one subsystem at a time
