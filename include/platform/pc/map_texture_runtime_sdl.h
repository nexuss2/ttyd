#pragma once

typedef struct PCMapTextureSet PCMapTextureSet;

PCMapTextureSet* PCMapTextureSetLoad(const char* map);
void PCMapTextureSetDrawSheet(PCMapTextureSet* set, int x, int y);
void PCMapTextureSetDestroy(PCMapTextureSet* set);
