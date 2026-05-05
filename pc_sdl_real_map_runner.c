#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_runtime_sdl.h"

int main(int argc, char** argv) {
    const char* map_name = "aaa_00";
    PCMapRuntime* map;

    if (argc > 1) {
        map_name = argv[1];
    }

    if (!PCRenderSDLInit(960, 720)) {
        return 1;
    }

    map = PCMapRuntimeLoad(map_name);
    if (!map) {
        PCRenderSDLShutdown();
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    PCMapRuntimeDrawWire(map);

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_real_map_runner.bmp");

    PCMapRuntimeDestroy(map);
    PCRenderSDLShutdown();

    return 0;
}
