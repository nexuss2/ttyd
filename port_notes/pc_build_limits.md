# PC Build Limits

Native cc cannot directly compile many real TTYD source files yet because they include Dolphin SDK headers that pull in GX, OS and Metrowerks-specific declarations.

Working native PC layer:
- file.c
- input.c
- time.c
- memory_pc.c
- dvdmgr_pc.c
- pc_main.c

Current strategy:
- keep original GameCube source building through ninja
- create standalone PC shims for managers as needed
- only later introduce PC-safe headers or larger shared abstractions
