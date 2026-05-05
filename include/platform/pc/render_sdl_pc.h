#pragma once

int PCRenderSDLInit(int width, int height);
void PCRenderSDLBeginFrame(void);
void PCRenderSDLClear(void);
void PCRenderSDLEndFrame(void);
int PCRenderSDLPollQuit(void);
void PCRenderSDLShutdown(void);
int PCRenderSDLSaveBMP(const char* path);
int PCRenderSDLDrawPPM(const char* path, int x, int y);
int PCRenderSDLDrawRGB(const unsigned char* pixels, int width, int height, int x, int y);
