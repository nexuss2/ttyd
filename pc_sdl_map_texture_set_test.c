#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_texture_runtime_sdl.h"

int main(int argc, char** argv) {
    const char* map = "aaa_00";
    char out_path[256];
    PCMapTextureSet* set;

    if (argc > 1) {
        map = argv[1];
    }

    sprintf(out_path, "build/pc/sdl_%s_texture_set_test.bmp", map);

    if (!PCRenderSDLInit(960, 760)) {
        return 1;
    }

    set = PCMapTextureSetLoad(map);
    if (!set) {
        PCRenderSDLShutdown();
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    PCMapTextureSetDrawSheet(set, 32, 32);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP(out_path);

    PCMapTextureSetDestroy(set);
    PCRenderSDLShutdown();

    printf("wrote %s\n", out_path);
    return 0;
}
