#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_texture_runtime_sdl.h"

int main(void) {
    PCMapTextureSet* set;

    if (!PCRenderSDLInit(800, 600)) {
        return 1;
    }

    set = PCMapTextureSetLoad("aaa_00");
    if (!set) {
        PCRenderSDLShutdown();
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    PCMapTextureSetDrawNamed(set, "aaa_yane", 80, 60, 256, 128);
    PCMapTextureSetDrawNamed(set, "aaa_moku_dot", 80, 190, 256, 256);
    PCMapTextureSetDrawNamed(set, "aaa_door", 170, 300, 96, 160);
    PCMapTextureSetDrawNamed(set, "aaa_dot", 420, 120, 256, 256);
    PCMapTextureSetDrawNamed(set, "aaa_kusa2", 0, 500, 800, 80);

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_fake_scene_test.bmp");

    PCMapTextureSetDestroy(set);
    PCRenderSDLShutdown();

    return 0;
}
