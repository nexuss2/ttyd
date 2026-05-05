# SDL Branch Progress

Branch: pc-render-sdl-window

## Confirmed

- SDL2 installs and builds in Codespaces.
- SDL window backend initialises successfully.
- Codespaces does not show the live window, but BMP screenshots verify output.
- SDL can clear and present frames.
- SDL can save BMP screenshots.
- SDL can draw:
  - raw RGB checker data
  - decoded CMPR texture data directly from memory
  - cached SDL textures
  - full map texture sheets

## Runtime texture path

Current working path:

    map texture palette TPL
    -> decode CMPR texture by index
    -> RGB pixel buffer
    -> cached SDL texture
    -> draw via SDL renderer
    -> save BMP screenshot

## Useful tests

    ./pc_sdl_build.sh

Individual outputs include:

    build/pc/sdl_window_test.bmp
    build/pc/sdl_rgb_test.bmp
    build/pc/sdl_cmpr_direct_test.bmp
    build/pc/sdl_cmpr_sheet_test.bmp
    build/pc/sdl_texture_cache_test.bmp
    build/pc/sdl_texture_runtime_test.bmp
    build/pc/sdl_map_texture_set_test.bmp
    build/pc/sdl_aaa_00_texture_set_test.bmp

## Next

- Add a cleaner SDL renderer API around map texture sets.
- Start linking texture names to texture indices.
- Begin drawing simple quads using selected map textures.
- Later, parse map geometry/materials and draw actual map surfaces.
