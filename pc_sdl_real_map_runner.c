#include <stdio.h>
#include <stdlib.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_runtime_sdl.h"

int main(int argc, char** argv) {
    const char* map_name = "aaa_00";
    float offset_x = 450.0f;
    float offset_y = 620.0f;
    float scale = 1.0f;
    PCMapRuntime* map;

    if (argc > 1) {
        map_name = argv[1];
    }

    if (argc > 2) {
        offset_x = (float)atof(argv[2]);
    }

    if (argc > 3) {
        offset_y = (float)atof(argv[3]);
    }

    if (argc > 4) {
        scale = (float)atof(argv[4]);
    }

    if (!PCRenderSDLInit(960, 720)) {
        return 1;
    }

    map = PCMapRuntimeLoad(map_name);
    if (!map) {
        PCRenderSDLShutdown();
        return 1;
    }

    PCMapRuntimeSetView(offset_x, offset_y, scale);

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    PCMapRuntimeDrawWire(map);

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_real_map_runner.bmp");

    PCMapRuntimeDestroy(map);
    PCRenderSDLShutdown();

    printf("rendered real map %s view offset=(%.2f, %.2f) scale=%.2f\n", map_name, offset_x, offset_y, scale);

    return 0;
}
