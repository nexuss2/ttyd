#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

static SDL_Window* s_window;
static SDL_Renderer* s_renderer;
static int s_width;
static int s_height;
static int s_frame;

int PCRenderSDLInit(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 0;
    }

    s_window = SDL_CreateWindow(
        "TTYD PC Port",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (!s_window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!s_renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(s_window);
        SDL_Quit();
        return 0;
    }

    s_width = width;
    s_height = height;
    s_frame = 0;
    printf("SDL render init %dx%d\n", width, height);
    return 1;
}

void PCRenderSDLBeginFrame(void) {
}

void PCRenderSDLClear(void) {
    SDL_SetRenderDrawColor(s_renderer, 20, 24, 32, 255);
    SDL_RenderClear(s_renderer);
}

void PCRenderSDLEndFrame(void) {
    SDL_RenderPresent(s_renderer);
    s_frame++;
}

int PCRenderSDLPollQuit(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            return 1;
        }
    }

    return 0;
}

void PCRenderSDLShutdown(void) {
    printf("SDL render shutdown after %d frames\n", s_frame);

    if (s_renderer) {
        SDL_DestroyRenderer(s_renderer);
        s_renderer = 0;
    }

    if (s_window) {
        SDL_DestroyWindow(s_window);
        s_window = 0;
    }

    SDL_Quit();
}

int PCRenderSDLSaveBMP(const char* path) {
    SDL_Surface* surface;

    if (!s_renderer) {
        return 0;
    }

    surface = SDL_CreateRGBSurfaceWithFormat(0, s_width, s_height, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!surface) {
        printf("SDL_CreateRGBSurfaceWithFormat failed: %s\n", SDL_GetError());
        return 0;
    }

    if (SDL_RenderReadPixels(s_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch) != 0) {
        printf("SDL_RenderReadPixels failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return 0;
    }

    if (SDL_SaveBMP(surface, path) != 0) {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return 0;
    }

    SDL_FreeSurface(surface);
    printf("saved %s\n", path);
    return 1;
}

int PCRenderSDLDrawPPM(const char* path, int x, int y) {
    FILE* f;
    char magic[3];
    int width;
    int height;
    int maxv;
    unsigned char* pixels;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect dst;

    if (!s_renderer) {
        return 0;
    }

    f = fopen(path, "rb");
    if (!f) {
        printf("failed to open ppm: %s\n", path);
        return 0;
    }

    if (fscanf(f, "%2s\n%d %d\n%d\n", magic, &width, &height, &maxv) != 4) {
        fclose(f);
        printf("bad ppm header: %s\n", path);
        return 0;
    }

    if (magic[0] != 'P' || magic[1] != '6' || maxv != 255) {
        fclose(f);
        printf("unsupported ppm: %s\n", path);
        return 0;
    }

    pixels = (unsigned char*)malloc(width * height * 3);
    if (!pixels) {
        fclose(f);
        return 0;
    }

    if (fread(pixels, 1, width * height * 3, f) != (size_t)(width * height * 3)) {
        free(pixels);
        fclose(f);
        printf("bad ppm data: %s\n", path);
        return 0;
    }

    fclose(f);

    surface = SDL_CreateRGBSurfaceFrom(
        pixels,
        width,
        height,
        24,
        width * 3,
        0x0000ff,
        0x00ff00,
        0xff0000,
        0
    );

    if (!surface) {
        free(pixels);
        printf("SDL_CreateRGBSurfaceFrom failed: %s\n", SDL_GetError());
        return 0;
    }

    texture = SDL_CreateTextureFromSurface(s_renderer, surface);
    SDL_FreeSurface(surface);
    free(pixels);

    if (!texture) {
        printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return 0;
    }

    dst.x = x;
    dst.y = y;
    dst.w = width;
    dst.h = height;

    SDL_RenderCopy(s_renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);

    return 1;
}

int PCRenderSDLDrawRGB(const unsigned char* pixels, int width, int height, int x, int y) {
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect dst;

    if (!s_renderer || !pixels || width <= 0 || height <= 0) {
        return 0;
    }

    surface = SDL_CreateRGBSurfaceFrom(
        (void*)pixels,
        width,
        height,
        24,
        width * 3,
        0x0000ff,
        0x00ff00,
        0xff0000,
        0
    );

    if (!surface) {
        printf("SDL_CreateRGBSurfaceFrom failed: %s\n", SDL_GetError());
        return 0;
    }

    texture = SDL_CreateTextureFromSurface(s_renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return 0;
    }

    dst.x = x;
    dst.y = y;
    dst.w = width;
    dst.h = height;

    SDL_RenderCopy(s_renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);

    return 1;
}

int PCRenderSDLDrawRGBScaled(const unsigned char* pixels, int width, int height, int x, int y, int draw_width, int draw_height) {
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect dst;

    if (!s_renderer || !pixels || width <= 0 || height <= 0 || draw_width <= 0 || draw_height <= 0) {
        return 0;
    }

    surface = SDL_CreateRGBSurfaceFrom(
        (void*)pixels,
        width,
        height,
        24,
        width * 3,
        0x0000ff,
        0x00ff00,
        0xff0000,
        0
    );

    if (!surface) {
        printf("SDL_CreateRGBSurfaceFrom failed: %s\n", SDL_GetError());
        return 0;
    }

    texture = SDL_CreateTextureFromSurface(s_renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return 0;
    }

    dst.x = x;
    dst.y = y;
    dst.w = draw_width;
    dst.h = draw_height;

    SDL_RenderCopy(s_renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);

    return 1;
}

typedef struct PCSDLTexture {
    SDL_Texture* texture;
    int width;
    int height;
} PCSDLTexture;

PCSDLTexture* PCRenderSDLCreateTextureRGB(const unsigned char* pixels, int width, int height) {
    SDL_Surface* surface;
    PCSDLTexture* out;

    if (!s_renderer || !pixels || width <= 0 || height <= 0) {
        return 0;
    }

    out = (PCSDLTexture*)malloc(sizeof(PCSDLTexture));
    if (!out) {
        return 0;
    }

    surface = SDL_CreateRGBSurfaceFrom(
        (void*)pixels,
        width,
        height,
        24,
        width * 3,
        0x0000ff,
        0x00ff00,
        0xff0000,
        0
    );

    if (!surface) {
        free(out);
        printf("SDL_CreateRGBSurfaceFrom failed: %s\n", SDL_GetError());
        return 0;
    }

    out->texture = SDL_CreateTextureFromSurface(s_renderer, surface);
    SDL_FreeSurface(surface);

    if (!out->texture) {
        free(out);
        printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return 0;
    }

    out->width = width;
    out->height = height;

    return out;
}

void PCRenderSDLDrawTexture(PCSDLTexture* texture, int x, int y, int draw_width, int draw_height) {
    SDL_Rect dst;

    if (!s_renderer || !texture || !texture->texture) {
        return;
    }

    dst.x = x;
    dst.y = y;
    dst.w = draw_width > 0 ? draw_width : texture->width;
    dst.h = draw_height > 0 ? draw_height : texture->height;

    SDL_RenderCopy(s_renderer, texture->texture, NULL, &dst);
}

void PCRenderSDLDestroyTexture(PCSDLTexture* texture) {
    if (!texture) {
        return;
    }

    if (texture->texture) {
        SDL_DestroyTexture(texture->texture);
    }

    free(texture);
}

SDL_Texture* PCRenderSDLTextureRaw(PCSDLTexture* texture) {
    if (!texture) {
        return 0;
    }

    return texture->texture;
}

SDL_Renderer* PCRenderSDLGetRenderer(void) {
    return s_renderer;
}
