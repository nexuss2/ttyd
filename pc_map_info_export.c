#include <stdio.h>

void mapInit(void);
int mapLoadPC(const char* map);
int mapWriteTextureNamesPC(const char* map, const char* out_path);

int main(int argc, char** argv) {
    char out[256];

    if (argc < 2) {
        printf("usage: %s map_name\n", argv[0]);
        return 1;
    }

    mapInit();

    if (!mapLoadPC(argv[1])) {
        printf("failed to load map %s\n", argv[1]);
        return 1;
    }

    sprintf(out, "build/pc/%s_texture_names.txt", argv[1]);

    if (!mapWriteTextureNamesPC(argv[1], out)) {
        printf("failed to write texture names\n");
        return 1;
    }

    return 0;
}
