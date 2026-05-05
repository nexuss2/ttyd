#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportFirstI4(const char* out_path, const void* data, u32 size);
int TPLExportFirstI8(const char* out_path, const void* data, u32 size);
int TPLExportFirstIA4(const char* out_path, const void* data, u32 size);
int TPLExportFirstRGB565(const char* out_path, const void* data, u32 size);
int TPLExportFirstCMPR(const char* out_path, const void* data, u32 size);

int main(void) {
    u32 length = 0;
    void* fade = arcOpen("fade.tpl", 0, &length);

    if (fade) {
        TPLExportFirstI4("build/pc/supported_fade_i4.ppm", fade, length);
        TPLExportFirstI8("build/pc/supported_fade_i8.ppm", fade, length);
    }

    length = 0;
    void* effect = arcOpen("effect.tpl", 0, &length);

    if (effect) {
        TPLExportFirstIA4("build/pc/supported_effect_ia4.ppm", effect, length);
    }

    length = 0;
    void* env = arcOpen("env.tpl", 0, &length);

    if (env) {
        TPLExportFirstRGB565("build/pc/supported_env_rgb565.ppm", env, length);
    }

    length = 0;
    void* map = arcOpen("./m/aaa_00/t", 0, &length);

    if (map) {
        TPLExportFirstCMPR("build/pc/supported_aaa_00_cmpr.ppm", map, length);
    }

    return 0;
}
