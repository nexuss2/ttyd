#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportAllCMPR(const char* out_dir, const char* prefix, const void* data, u32 size);
int TPLExportFirstI4(const char* out_path, const void* data, u32 size);
int TPLExportFirstI8(const char* out_path, const void* data, u32 size);
int TPLExportFirstIA4(const char* out_path, const void* data, u32 size);
int TPLExportFirstRGB565(const char* out_path, const void* data, u32 size);

int main(int argc, char** argv) {
    u32 length = 0;
    void* data;
    const char* path;
    const char* prefix;

    if (argc < 3) {
        printf("usage: %s texture_path prefix\n", argv[0]);
        printf("example: %s ./m/aaa_00/t aaa_00\n", argv[0]);
        return 1;
    }

    path = argv[1];
    prefix = argv[2];

    data = arcOpen(path, 0, &length);
    if (!data) {
        printf("failed to load %s\n", path);
        return 1;
    }

    printf("loaded %s length=%u\n", path, length);

    TPLExportAllCMPR("build/pc", prefix, data, length);
    TPLExportFirstI4("build/pc/export_i4.ppm", data, length);
    TPLExportFirstI8("build/pc/export_i8.ppm", data, length);
    TPLExportFirstIA4("build/pc/export_ia4.ppm", data, length);
    TPLExportFirstRGB565("build/pc/export_rgb565.ppm", data, length);

    return 0;
}
