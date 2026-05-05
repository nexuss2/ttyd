#pragma once
#include <SDL.h>

typedef struct PCTextureRuntime PCTextureRuntime;

PCTextureRuntime* PCTextureRuntimeCreateFromTPLCMPR(const void* tpl, unsigned int size, int index);
void PCTextureRuntimeDraw(PCTextureRuntime* texture, int x, int y, int draw_width, int draw_height);
SDL_Texture* PCTextureRuntimeGetSDLTexture(PCTextureRuntime* texture);
void PCTextureRuntimeDestroy(PCTextureRuntime* texture);
