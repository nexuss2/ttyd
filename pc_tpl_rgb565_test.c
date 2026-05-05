#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportFirstRGB565(const char* out_path, const void* data, u32 size);

int main(void) {
    u32 length = 0;
    void* data = arcOpen("env.tpl", 0, &length);

    if (!data) {
        printf("failed to load env.tpl\n");
        return 1;
    }

    if (!TPLExportFirstRGB565("build/pc/env_rgb565.ppm", data, length)) {
        printf("RGB565 export failed\n");
        return 1;
    }

    return 0;
}
