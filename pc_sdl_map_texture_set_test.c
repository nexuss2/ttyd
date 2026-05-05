#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_texture_runtime_sdl.h"

int main(void) {
    PCMapTextureSet* set;

    if (!PCRenderSDLInit(960, 760)) {
        return 1;
    }

    set = PCMapTextureSetLoad("aaa_00");
    if (!set) {
        PCRenderSDLShutdown();
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    PCMapTextureSetDrawSheet(set, 32, 32);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_map_texture_set_test.bmp");

    PCMapTextureSetDestroy(set);
    PCRenderSDLShutdown();

    return 0;
}
