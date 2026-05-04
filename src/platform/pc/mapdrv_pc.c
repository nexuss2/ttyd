#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

static void* s_map_dat;
static void* s_map_tpl;
static u32 s_map_dat_size;
static u32 s_map_tpl_size;

void mapInit(void) {
    s_map_dat = 0;
    s_map_tpl = 0;
    s_map_dat_size = 0;
    s_map_tpl_size = 0;
}

int mapLoadPC(const char* map) {
    char path[128];

    sprintf(path, "./m/%s/d", map);
    s_map_dat = arcOpen(path, 0, &s_map_dat_size);

    sprintf(path, "./m/%s/t", map);
    s_map_tpl = arcOpen(path, 0, &s_map_tpl_size);

    printf("mapLoadPC %s dat=%p size=%u tpl=%p size=%u\n",
           map,
           s_map_dat,
           s_map_dat_size,
           s_map_tpl,
           s_map_tpl_size);

    return s_map_dat != 0;
}
