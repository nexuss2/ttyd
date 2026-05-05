#pragma once

typedef struct PCMapTextureSet PCMapTextureSet;

PCMapTextureSet* PCMapTextureSetLoad(const char* map);
void PCMapTextureSetDrawSheet(PCMapTextureSet* set, int x, int y);
int PCMapTextureSetFindIndex(PCMapTextureSet* set, const char* name);
void PCMapTextureSetDrawNamed(PCMapTextureSet* set, const char* name, int x, int y, int draw_width, int draw_height);
void PCMapTextureSetDestroy(PCMapTextureSet* set);
