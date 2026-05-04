#include <stdio.h>

typedef unsigned int u32;

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

int PlatformFileExists(const char* path);
int PlatformFileOpen(const char* path, DVDFileInfo* info);

static void test_file(const char* path) {
    DVDFileInfo info;

    if (!PlatformFileExists(path)) {
        printf("missing: %s\n", path);
        return;
    }

    if (!PlatformFileOpen(path, &info)) {
        printf("open failed: %s\n", path);
        return;
    }

    printf("ok: %s length=%u\n", path, info.length);
}

int main(void) {
    test_file("./m/aaa_00/d");
    test_file("./m/aaa_00/t");
    test_file("./m/aaa_00/s");
    test_file("./m/aaa_00/c");
    return 0;
}
