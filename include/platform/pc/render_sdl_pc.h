#pragma once
#include <SDL.h>

typedef struct PCSDLTexture PCSDLTexture;

int PCRenderSDLInit(int width, int height);
void PCRenderSDLBeginFrame(void);
void PCRenderSDLClear(void);
void PCRenderSDLEndFrame(void);
int PCRenderSDLPollQuit(void);
void PCRenderSDLShutdown(void);
int PCRenderSDLSaveBMP(const char* path);

int PCRenderSDLDrawPPM(const char* path, int x, int y);
int PCRenderSDLDrawRGB(const unsigned char* pixels, int width, int height, int x, int y);
int PCRenderSDLDrawRGBScaled(const unsigned char* pixels, int width, int height, int x, int y, int draw_width, int draw_height);

PCSDLTexture* PCRenderSDLCreateTextureRGB(const unsigned char* pixels, int width, int height);
void PCRenderSDLDrawTexture(PCSDLTexture* texture, int x, int y, int draw_width, int draw_height);
void PCRenderSDLDestroyTexture(PCSDLTexture* texture);
SDL_Texture* PCRenderSDLTextureRaw(PCSDLTexture* texture);

SDL_Renderer* PCRenderSDLGetRenderer(void);
