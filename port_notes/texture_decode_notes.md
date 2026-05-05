# Texture Decode Notes

Confirmed TEXPalette format:
- version: 0x0020AF30
- descriptor count and descriptor array parsing works
- texture headers parse width, height, format, data offset, wrapS, wrapT

Working PC texture exports:
- GX_TF_I4 / format 0
- GX_TF_I8 / format 1
- GX_TF_IA4 / format 2
- GX_TF_RGB565 / format 5
- GX_TF_CMPR / format 14

Working outputs:
- fade_i4.ppm / fade_i4.png
- fade_i8.ppm / fade_i8.png
- effect_ia4.ppm / effect_ia4.png
- env_rgb565.ppm / env_rgb565.png
- aaa_00_cmpr.ppm / aaa_00_cmpr.png
- all CMPR textures from ./m/aaa_00/t

Notes:
- CMPR export appears visually valid.
- I4 needed tiled layout handling.
- PPM is used as the simple native output format.
- PNG conversion is handled by tools/ppm_to_png.py without PIL.
