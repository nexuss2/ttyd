#pragma once

#include <SDL.h>

typedef struct PCMapTextureSet PCMapTextureSet;

PCMapTextureSet* PCMapTextureSetLoad(const char* map);
SDL_Texture* PCMapTextureSetFindTexture(PCMapTextureSet* set, const char* name);
void PCMapTextureSetDestroy(PCMapTextureSet* set);
