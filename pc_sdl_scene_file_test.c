#include <stdio.h>
#include <string.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_texture_runtime_sdl.h"

int main(int argc, char** argv) {
    const char* scene_path = "pc_scenes/aaa_house.scene";
    char map[128] = "aaa_00";
    PCMapTextureSet* set;
    FILE* f;
    char name[128];
    int x;
    int y;
    int w;
    int h;

    if (argc > 1) {
        scene_path = argv[1];
    }

    f = fopen(scene_path, "r");
    if (!f) {
        printf("failed to open scene file: %s\n", scene_path);
        return 1;
    }

    while (fscanf(f, "%127s", name) == 1) {
        if (strcmp(name, "map") == 0) {
            fscanf(f, "%127s", map);
            break;
        }
    }

    fclose(f);

    if (!PCRenderSDLInit(800, 600)) {
        return 1;
    }

    set = PCMapTextureSetLoad(map);
    if (!set) {
        PCRenderSDLShutdown();
        return 1;
    }

    f = fopen(scene_path, "r");
    if (!f) {
        PCMapTextureSetDestroy(set);
        PCRenderSDLShutdown();
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    while (fscanf(f, "%127s", name) == 1) {
        if (strcmp(name, "map") == 0) {
            fscanf(f, "%127s", map);
            continue;
        }

        if (name[0] == '#') {
            char buffer[512];
            fgets(buffer, sizeof(buffer), f);
            continue;
        }

        if (fscanf(f, "%d %d %d %d", &x, &y, &w, &h) == 4) {
            PCMapTextureSetDrawNamed(set, name, x, y, w, h);
        }
    }

    fclose(f);

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_scene_file_test.bmp");

    PCMapTextureSetDestroy(set);
    PCRenderSDLShutdown();

    return 0;
}
