#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_runtime_sdl.h"

static int run_once(const char* map_name, int use_auto, float offset_x, float offset_y, float scale) {
    PCMapRuntime* map;

    if (!PCRenderSDLInit(960, 720)) {
        return 1;
    }

    map = PCMapRuntimeLoad(map_name);
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
    PCMapRuntimeDrawWire(map);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_real_map_runner.bmp");

    PCMapRuntimeDestroy(map);
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

    if (!PCRenderSDLInit(960, 720)) {
        return 1;
    }

    map = PCMapRuntimeLoad(map_name);
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
    PCRenderSDLShutdown();

    return 0;
}

int main(int argc, char** argv) {
    const char* map_name = "aaa_00";
    float offset_x = 450.0f;
    float offset_y = 620.0f;
    float scale = 1.0f;
    int use_auto = 0;

    if (argc > 1) {
        map_name = argv[1];
    }

    if (argc > 2 && strcmp(argv[2], "--live") == 0) {
        return run_live(map_name);
    }

    if (argc > 2 && strcmp(argv[2], "auto") == 0) {
        use_auto = 1;
    } else {
        if (argc > 2) {
            offset_x = (float)atof(argv[2]);
        }

        if (argc > 3) {
            offset_y = (float)atof(argv[3]);
        }

        if (argc > 4) {
            scale = (float)atof(argv[4]);
        }
    }

    return run_once(map_name, use_auto, offset_x, offset_y, scale);
}
