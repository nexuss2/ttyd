#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

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

    return s_map_d != 0 && s_map_t != 0 && s_map_s != 0 && s_map_c != 0;
}
