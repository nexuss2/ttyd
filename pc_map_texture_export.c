#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportAllCMPR(const char* out_dir, const char* prefix, const void* data, u32 size);

int main(int argc, char** argv) {
    char path[128];
    const char* map;
    u32 length = 0;
    void* data;

    if (argc < 2) {
        printf("usage: %s map_name\n", argv[0]);
        printf("example: %s aaa_00\n", argv[0]);
        return 1;
    }

    map = argv[1];

    sprintf(path, "./m/%s/t", map);

    data = arcOpen(path, 0, &length);
    if (!data) {
        printf("failed to load %s\n", path);
        return 1;
    }

    printf("loaded map texture palette %s length=%u\n", path, length);

    if (!TPLExportAllCMPR("build/pc", map, data, length)) {
        printf("no CMPR textures exported for %s\n", map);
        return 1;
    }

    return 0;
}
