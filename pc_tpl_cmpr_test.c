#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportFirstCMPR(const char* out_path, const void* data, u32 size);

int main(void) {
    u32 length = 0;
    void* data = arcOpen("./m/aaa_00/t", 0, &length);

    if (!data) {
        printf("failed to load map texture\n");
        return 1;
    }

    if (!TPLExportFirstCMPR("build/pc/aaa_00_cmpr.ppm", data, length)) {
        printf("CMPR export failed\n");
        return 1;
    }

    return 0;
}
