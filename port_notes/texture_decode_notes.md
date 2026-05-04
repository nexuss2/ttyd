# Texture Decode Notes

Confirmed:
- TEXPalette version is 0x0020AF30.
- TPL/TEXPalette descriptor parsing works.
- GX_TF_CMPR is format 14.
- PC CMPR export to PPM/PNG works for map texture ./m/aaa_00/t.
- Generated aaa_00_cmpr.png appears as a valid texture tile.

Needs work:
- GX_TF_I4 format 0 currently exports but appears striped because GameCube I4 texture data is tiled/swizzled, not simple linear pixels.
- Need tiled decode for I4 and other formats.
- Need export helpers for all descriptors, not only the first matching texture.
