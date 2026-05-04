#include <stdio.h>
#include <string.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);


static u32 read_be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}



static int find_map_chunk(const void* data, u32 size, const char* wanted, u32* outOffset) {
    const unsigned char* p;
    u32 dataSize;
    u32 relCount;
    u32 chunkCount;
    u32 chunkOffset;
    u32 stringOffset;
    u32 i;

    if (!data || size < 0x20) {
        return 0;
    }

    p = (const unsigned char*)data;
    dataSize = read_be32(p + 4);
    relCount = read_be32(p + 8);
    chunkCount = read_be32(p + 12);

    chunkOffset = 0x20 + dataSize + (relCount * 4);
    stringOffset = chunkOffset + (chunkCount * 8);

    if (chunkOffset + chunkCount * 8 > size) {
        return 0;
    }

    for (i = 0; i < chunkCount; i++) {
        u32 off = read_be32(p + chunkOffset + i * 8);
        u32 str = read_be32(p + chunkOffset + i * 8 + 4);
        const char* name;

        if (stringOffset + str >= size) {
            continue;
        }

        name = (const char*)(p + stringOffset + str);

        if (strcmp(name, wanted) == 0) {
            *outOffset = off;
            return 1;
        }
    }

    return 0;
}




static const char* map_string_at(const void* data, u32 size, u32 target) {
    const unsigned char* base;

    if (!data || 0x20 + target >= size) {
        return "<outside>";
    }

    base = (const unsigned char*)data + 0x20;
    return (const char*)(base + target);
}



static void dump_texture_table(const void* data, u32 size) {
    const unsigned char* base;
    const unsigned char* table;
    u32 off;
    u32 i;

    if (!find_map_chunk(data, size, "texture_table", &off)) {
        printf("texture_table missing\n");
        return;
    }

    if (0x20 + off >= size) {
        printf("texture_table outside file\n");
        return;
    }

    base = (const unsigned char*)data + 0x20;
    table = base + off;

    printf("texture_table offset=%u preview:\n", off);

    for (i = 0; i < 64; i++) {
        u32 target = read_be32(table + i * 4);

        if (target == 0) {
            printf("  texture[%u] null\n", i);
        } else if (0x20 + target < size) {
            printf("  texture[%u] target=%u str=%s\n", i, target, (const char*)(base + target));
        } else {
            printf("  texture[%u] outside target=%u\n", i, target);
        }
    }
}

static void dump_material_name_table(const void* data, u32 size) {
    const unsigned char* base;
    const unsigned char* table;
    u32 off;
    u32 i;

    if (!find_map_chunk(data, size, "material_name_table", &off)) {
        printf("material_name_table missing\n");
        return;
    }

    if (0x20 + off >= size) {
        printf("material_name_table outside file\n");
        return;
    }

    base = (const unsigned char*)data + 0x20;
    table = base + off;

    printf("material_name_table offset=%u preview:\n", off);

    for (i = 0; i < 16; i++) {
        u32 target = read_be32(table + i * 4);

        if (target == 0) {
            printf("  material[%u] null\n", i);
        } else if (0x20 + target < size) {
            printf("  material[%u] %s\n", i, (const char*)(base + target));
        } else {
            printf("  material[%u] outside target=%u\n", i, target);
        }
    }
}

static void print_map_information_summary(const void* data, u32 size) {
    const unsigned char* base;
    const unsigned char* info;
    u32 off;

    if (!find_map_chunk(data, size, "information", &off)) {
        printf("map information summary unavailable\n");
        return;
    }

    base = (const unsigned char*)data + 0x20;
    info = base + off;

    printf("map information summary:\n");
    printf("  version: %s\n", map_string_at(data, size, read_be32(info + 0)));
    printf("  scene: %s\n", map_string_at(data, size, read_be32(info + 8)));
    printf("  area: %s\n", map_string_at(data, size, read_be32(info + 12)));
    printf("  build: %s\n", map_string_at(data, size, read_be32(info + 16)));
    printf("  object_a: %s\n", map_string_at(data, size, read_be32(info + 20)));
    printf("  object_b: %s\n", map_string_at(data, size, read_be32(info + 36)));
}

static void dump_information_offsets(const void* data, u32 size) {
    const unsigned char* base;
    const unsigned char* info;
    u32 off;
    u32 i;

    if (!find_map_chunk(data, size, "information", &off)) {
        return;
    }

    base = (const unsigned char*)data + 0x20;
    info = base + off;

    printf("information offset targets:\n");
    for (i = 0; i < 12; i++) {
        u32 target = read_be32(info + i * 4);
        if (0x20 + target < size) {
            const char* maybe = (const char*)(base + target);
            printf("  [%u] target=%u str=%s\n", i, target, maybe);
        } else {
            printf("  [%u] target=%u outside\n", i, target);
        }
    }
}

static void dump_information_chunk(const void* data, u32 size) {
    const unsigned char* p;
    u32 off;
    u32 i;

    if (!find_map_chunk(data, size, "information", &off)) {
        printf("information chunk missing\n");
        return;
    }

    if (off >= size) {
        printf("information chunk offset outside file\n");
        return;
    }

    p = (const unsigned char*)data + 0x20 + off;

    printf("information chunk dataOffset=%u fileOffset=%u first bytes:", off, 0x20 + off);
    for (i = 0; i < 64 && 0x20 + off + i < size; i++) {
        printf(" %02x", p[i]);
    }
    printf("\n");

    printf("information strings guess: %s\n", (const char*)p);
}

static void dump_named_chunks(const void* data, u32 size) {
    const char* names[] = {
        "information",
        "texture_table",
        "material_name_table",
        "vcd_table",
        "animation_table",
        "curve_table",
        "fog_table",
        "light_table"
    };
    u32 i;
    u32 off;

    for (i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        if (find_map_chunk(data, size, names[i], &off)) {
            printf("named chunk %s offset=%u\n", names[i], off);
        } else {
            printf("named chunk %s missing\n", names[i]);
        }
    }
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
     //   /* dump_bytes("map d", s_map_d, s_map_d_size); */
     //   /* dump_map_header(s_map_d, s_map_d_size); */
     //   /* dump_map_chunks(s_map_d, s_map_d_size); */
    //   /* dump_named_chunks(s_map_d, s_map_d_size); */
      //  /* dump_information_chunk(s_map_d, s_map_d_size); */
     //  /* dump_information_offsets(s_map_d, s_map_d_size); */
      print_map_information_summary(s_map_d, s_map_d_size);
        dump_material_name_table(s_map_d, s_map_d_size);
        dump_texture_table(s_map_d, s_map_d_size);
    }

    return s_map_d != 0 && s_map_t != 0 && s_map_s != 0 && s_map_c != 0;
}
