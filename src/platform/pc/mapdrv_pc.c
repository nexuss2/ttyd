#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);


static u32 read_be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}


static void dump_map_chunks(const void* data, u32 size) {
    const unsigned char* p;
    u32 dataSize;
    u32 relCount;
    u32 chunkCount;
    u32 chunkOffset;
    u32 stringOffset;
    u32 i;

    if (!data || size < 0x20) {
        return;
    }

    p = (const unsigned char*)data;
    dataSize = read_be32(p + 4);
    relCount = read_be32(p + 8);
    chunkCount = read_be32(p + 12);

    chunkOffset = 0x20 + dataSize + (relCount * 4);
    stringOffset = chunkOffset + (chunkCount * 8);

    printf("map layout dataOff=32 relOff=%u chunkOff=%u stringOff=%u\n",
           0x20 + dataSize,
           chunkOffset,
           stringOffset);

    if (chunkOffset + chunkCount * 8 > size) {
        printf("chunk table outside file\n");
        return;
    }

    for (i = 0; i < chunkCount && i < 8; i++) {
        u32 off = read_be32(p + chunkOffset + i * 8);
        u32 str = read_be32(p + chunkOffset + i * 8 + 4);
        const char* name = "";

        if (stringOffset + str < size) {
            name = (const char*)(p + stringOffset + str);
        }

        printf("chunk[%u] offset=%u string=%u name=%s\n", i, off, str, name);
    }
}

static void dump_map_header(const void* data, u32 size) {
    const unsigned char* p;

    if (!data || size < 16) {
        printf("map header unavailable\n");
        return;
    }

    p = (const unsigned char*)data;

    printf("map header fileSize=%u dataSize=%u relCount=%u unk0c=%u\n",
           read_be32(p + 0),
           read_be32(p + 4),
           read_be32(p + 8),
           read_be32(p + 12));
}

static void dump_bytes(const char* label, const void* data, u32 size) {
    const unsigned char* p;
    u32 i;

    p = (const unsigned char*)data;

    printf("%s first bytes:", label);
    for (i = 0; i < size && i < 32; i++) {
        printf(" %02x", p[i]);
    }
    printf("\n");
}


static void* s_map_d;
static void* s_map_t;
static void* s_map_s;
static void* s_map_c;

static u32 s_map_d_size;
static u32 s_map_t_size;
static u32 s_map_s_size;
static u32 s_map_c_size;

static void* load_map_file(const char* map, const char* suffix, u32* size) {
    char path[128];
    void* data;

    sprintf(path, "./m/%s/%s", map, suffix);
    data = arcOpen(path, 0, size);

    printf("map file %s size=%u loaded=%d\n", path, *size, data != 0);

    return data;
}

void mapInit(void) {
    s_map_d = 0;
    s_map_t = 0;
    s_map_s = 0;
    s_map_c = 0;

    s_map_d_size = 0;
    s_map_t_size = 0;
    s_map_s_size = 0;
    s_map_c_size = 0;
}

int mapLoadPC(const char* map) {
    s_map_d = load_map_file(map, "d", &s_map_d_size);
    s_map_t = load_map_file(map, "t", &s_map_t_size);
    s_map_s = load_map_file(map, "s", &s_map_s_size);
    s_map_c = load_map_file(map, "c", &s_map_c_size);

    printf("mapLoadPC %s d=%u t=%u s=%u c=%u\n",
           map,
           s_map_d_size,
           s_map_t_size,
           s_map_s_size,
           s_map_c_size);

    if (s_map_d) {
        dump_bytes("map d", s_map_d, s_map_d_size);
        dump_map_header(s_map_d, s_map_d_size);
        dump_map_chunks(s_map_d, s_map_d_size);
    }

    return s_map_d != 0 && s_map_t != 0 && s_map_s != 0 && s_map_c != 0;
}
