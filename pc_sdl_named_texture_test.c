#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_texture_runtime_sdl.h"

int main(void) {
    PCMapTextureSet* set;

    if (!PCRenderSDLInit(640, 480)) {
        return 1;
    }

    set = PCMapTextureSetLoad("aaa_00");
    if (!set) {
        PCRenderSDLShutdown();
        return 1;
    }

    printf("aaa_door index=%d\n", PCMapTextureSetFindIndex(set, "aaa_door"));
    printf("aaa_dot index=%d\n", PCMapTextureSetFindIndex(set, "aaa_dot"));

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    PCMapTextureSetDrawNamed(set, "aaa_door", 160, 120, 128, 128);
    PCMapTextureSetDrawNamed(set, "aaa_dot", 352, 120, 128, 128);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_named_texture_test.bmp");

    PCMapTextureSetDestroy(set);
    PCRenderSDLShutdown();

    return 0;
}
