#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportAllCMPR(const char* out_dir, const char* prefix, const void* data, u32 size);

int main(void) {
    u32 length = 0;
    void* data = arcOpen("./m/aaa_00/t", 0, &length);

    if (!data) {
        printf("failed to load map texture palette\n");
        return 1;
    }

    if (!TPLExportAllCMPR("build/pc", "aaa_00_cmpr", data, length)) {
        printf("no CMPR textures exported\n");
        return 1;
    }

    return 0;
}
