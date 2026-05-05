#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/texture_runtime_sdl.h"

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

int main(void) {
    u32 length = 0;
    void* tpl;
    PCTextureRuntime* textures[31];
    int i;

    tpl = arcOpen("./m/aaa_00/t", 0, &length);
    if (!tpl) {
        printf("failed to load map texture palette\n");
        return 1;
    }

    if (!PCRenderSDLInit(960, 640)) {
        return 1;
    }

    for (i = 0; i < 31; i++) {
        textures[i] = PCTextureRuntimeCreateFromTPLCMPR(tpl, length, i);
        if (!textures[i]) {
            printf("failed texture %d\n", i);
        }
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    for (i = 0; i < 31; i++) {
        int col = i % 6;
        int row = i / 6;
        int x = 32 + col * 150;
        int y = 32 + row * 110;

        if (textures[i]) {
            PCTextureRuntimeDraw(textures[i], x, y, 96, 96);
        }
    }

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_runtime_texture_sheet_test.bmp");

    for (i = 0; i < 31; i++) {
        PCTextureRuntimeDestroy(textures[i]);
    }

    PCRenderSDLShutdown();

    return 0;
}
