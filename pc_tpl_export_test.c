#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportFirstI4(const char* out_path, const void* data, u32 size);

int main(void) {
    u32 length = 0;
    void* data = arcOpen("fade.tpl", 0, &length);

    if (!data) {
        printf("failed to load fade.tpl\n");
        return 1;
    }

    if (!TPLExportFirstI4("build/pc/fade_i4.ppm", data, length)) {
        printf("export failed\n");
        return 1;
    }

    return 0;
}
