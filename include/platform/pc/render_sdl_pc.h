#pragma once

int PCRenderSDLInit(int width, int height);
void PCRenderSDLBeginFrame(void);
void PCRenderSDLClear(void);
void PCRenderSDLEndFrame(void);
int PCRenderSDLPollQuit(void);
void PCRenderSDLShutdown(void);
