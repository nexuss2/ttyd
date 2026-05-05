#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/texture_runtime_sdl.h"

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

int main(void) {
    u32 length = 0;
    void* tpl;
    PCTextureRuntime* texture;

    tpl = arcOpen("./m/aaa_00/t", 0, &length);
    if (!tpl) {
        printf("failed to load map texture palette\n");
        return 1;
    }

    if (!PCRenderSDLInit(640, 480)) {
        return 1;
    }

    texture = PCTextureRuntimeCreateFromTPLCMPR(tpl, length, 0);
    if (!texture) {
        PCRenderSDLShutdown();
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    PCTextureRuntimeDraw(texture, 256, 176, 128, 128);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_texture_runtime_test.bmp");

    PCTextureRuntimeDestroy(texture);
    PCRenderSDLShutdown();

    return 0;
}
