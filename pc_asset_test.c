#include <stdio.h>

typedef unsigned int u32;
typedef int s32;

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
    test_file("/env.tpl");
    test_file("/fade.tpl");
    test_file("/f/msgWindow.tpl");
    test_file("/e/jp/effect.tpl");
    test_file("/rel/aaa.rel");
    return 0;
}
