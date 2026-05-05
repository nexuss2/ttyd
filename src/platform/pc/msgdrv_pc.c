#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int u32;

typedef struct PCMessageEntry {
    char* tag;
    char* text;
} PCMessageEntry;

static char* s_msg_data;
static u32 s_msg_length;
static PCMessageEntry* s_entries;
static u32 s_entry_count;

int PlatformFileExists(const char* path);
int PlatformFileOpen(const char* path, void* info);
int PlatformFileRead(const char* path, void* info, void* address, u32 size, int offset);

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

static void clear_messages(void) {
    if (s_msg_data) {
        free(s_msg_data);
    }

    if (s_entries) {
        free(s_entries);
    }

    s_msg_data = 0;
    s_msg_length = 0;
    s_entries = 0;
    s_entry_count = 0;
}

int msgLoadPC(const char* filename) {
    char path[128];
    DVDFileInfo info;
    char* p;
    u32 count;
    u32 i;

    clear_messages();

    sprintf(path, "./msg/%s.txt", filename);

    if (!PlatformFileExists(path)) {
        printf("message file missing: %s\n", path);
        return 0;
    }

    if (!PlatformFileOpen(path, &info)) {
        printf("message open failed: %s\n", path);
        return 0;
    }

    s_msg_data = (char*)malloc(info.length + 1);
    if (!s_msg_data) {
        return 0;
    }

    if (PlatformFileRead(path, &info, s_msg_data, info.length, 0) < 0) {
        clear_messages();
        return 0;
    }

    s_msg_data[info.length] = 0;
    s_msg_length = info.length;

    p = s_msg_data;
    count = 0;

    while ((u32)(p - s_msg_data) < s_msg_length && *p) {
        count++;
        p += strlen(p) + 1;
    }

    s_entry_count = count / 2;
    s_entries = (PCMessageEntry*)calloc(s_entry_count, sizeof(PCMessageEntry));
    if (!s_entries) {
        clear_messages();
        return 0;
    }

    p = s_msg_data;
    for (i = 0; i < s_entry_count; i++) {
        s_entries[i].tag = p;
        p += strlen(p) + 1;

        s_entries[i].text = p;
        p += strlen(p) + 1;
    }

    printf("msgLoadPC %s entries=%u length=%u\n", filename, s_entry_count, s_msg_length);
    return 1;
}

const char* msgSearchPC(const char* tag) {
    u32 i;

    for (i = 0; i < s_entry_count; i++) {
        if (strcmp(s_entries[i].tag, tag) == 0) {
            return s_entries[i].text;
        }
    }

    return 0;
}

void msgDumpPC(u32 max_entries) {
    u32 i;

    for (i = 0; i < s_entry_count && i < max_entries; i++) {
        printf("[%u] %s = %s\n", i, s_entries[i].tag, s_entries[i].text);
    }
}
