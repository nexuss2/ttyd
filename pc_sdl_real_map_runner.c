#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_runtime_sdl.h"
#include "platform/pc/map_texture_runtime_sdl.h"

void mapSetPCDebug(int enabled);

static int run_once(const char* map_name, int use_auto, int draw_filled, float offset_x, float offset_y, float scale) {
    PCMapRuntime* map;
    PCMapTextureSet* textures;

    if (!PCRenderSDLInit(960, 720)) {
        return 1;
    }

    map = PCMapRuntimeLoad(map_name);
    textures = PCMapTextureSetLoad(map_name);
    if (!map) {
        PCRenderSDLShutdown();
        return 1;
    }

    if (use_auto) {
        PCMapRuntimeUseAutoView(map, 960, 720);
    } else {
        PCMapRuntimeSetView(offset_x, offset_y, scale);
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    if (draw_filled) {
        PCMapRuntimeDrawFilled(map, textures);
    }
    PCMapRuntimeDrawWire(map);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_real_map_runner.bmp");

    PCMapRuntimeDestroy(map);
    PCMapTextureSetDestroy(textures);
    PCRenderSDLShutdown();

    printf("saved real map %s mode=%s offset=(%.2f, %.2f) scale=%.2f\n",
        map_name,
        use_auto ? "auto" : "manual",
        offset_x,
        offset_y,
        scale);

    return 0;
}

static int run_live(const char* map_name) {
    float offset_x = 450.0f;
    float offset_y = 620.0f;
    float scale = 1.0f;
    int frame = 0;
    int running = 1;
    PCMapRuntime* map;
    PCMapTextureSet* textures;

    if (!PCRenderSDLInit(960, 720)) {
        return 1;
    }

    map = PCMapRuntimeLoad(map_name);
    textures = PCMapTextureSetLoad(map_name);
    if (!map) {
        PCRenderSDLShutdown();
        return 1;
    }

    printf("LIVE MODE: click the SDL window first. Arrow keys pan, Q/E zoom, ESC quit, S screenshot.\n");

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = 0;
                        break;
                    case SDLK_LEFT:
                        offset_x -= 25.0f;
                        break;
                    case SDLK_RIGHT:
                        offset_x += 25.0f;
                        break;
                    case SDLK_UP:
                        offset_y -= 25.0f;
                        break;
                    case SDLK_DOWN:
                        offset_y += 25.0f;
                        break;
                    case SDLK_q:
                        scale *= 0.9f;
                        break;
                    case SDLK_e:
                        scale *= 1.1f;
                        break;
                    case SDLK_s:
                        PCRenderSDLSaveBMP("build/pc/sdl_real_map_runner.bmp");
                        printf("saved screenshot\n");
                        break;
                    default:
                        break;
                }
            }
        }

        PCMapRuntimeSetView(offset_x, offset_y, scale);

        PCRenderSDLBeginFrame();
        PCRenderSDLClear();
        PCMapRuntimeDrawWire(map);
        PCRenderSDLEndFrame();

        if (frame == 0) {
            PCRenderSDLSaveBMP("build/pc/sdl_real_map_runner.bmp");
        }

        SDL_Delay(16);
        frame++;
    }

    PCMapRuntimeDestroy(map);
    PCMapTextureSetDestroy(textures);
    PCRenderSDLShutdown();

    return 0;
}

int main(int argc, char** argv) {
    const char* map_name = "aaa_00";
    float offset_x = 450.0f;
    float offset_y = 620.0f;
    float scale = 1.0f;
    int use_auto = 0;
    int draw_filled = 0;
    int debug_map = 0;
    int i;

    if (argc > 1) {
        map_name = argv[1];
    }

    for (i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--debug-map") == 0) {
            debug_map = 1;
        }

        if (strcmp(argv[i], "--filled") == 0) {
            draw_filled = 1;
        }
    }

    mapSetPCDebug(debug_map);

    if (argc > 2 && strcmp(argv[2], "--live") == 0) {
        return run_live(map_name);
    }

    if (argc > 2 && strcmp(argv[2], "auto") == 0) {
        use_auto = 1;
    } else {
        if (argc > 2 && argv[2][0] != '-') {
            offset_x = (float)atof(argv[2]);
        }

        if (argc > 3 && argv[3][0] != '-') {
            offset_y = (float)atof(argv[3]);
        }

        if (argc > 4 && argv[4][0] != '-') {
            scale = (float)atof(argv[4]);
        }
    }

    if (draw_filled) {
        printf("filled flag set, using filled real geometry path\n");
    }

    return run_once(map_name, use_auto, draw_filled, offset_x, offset_y, scale);
}
