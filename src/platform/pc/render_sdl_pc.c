#include <stdio.h>
#include <SDL2/SDL.h>

static SDL_Window* s_window;
static SDL_Renderer* s_renderer;
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

    s_frame = 0;
    printf("SDL render init %dx%d\n", width, height);
    return 1;
}

void PCRenderSDLBeginFrame(void) {
}

void PCRenderSDLClear(void) {
    SDL_Rect rect;

    SDL_SetRenderDrawColor(s_renderer, 20, 24, 32, 255);
    SDL_RenderClear(s_renderer);

    rect.x = 220;
    rect.y = 140;
    rect.w = 200;
    rect.h = 160;

    SDL_SetRenderDrawColor(s_renderer, 180, 90, 40, 255);
    SDL_RenderFillRect(s_renderer, &rect);
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

    surface = SDL_CreateRGBSurfaceWithFormat(0, 640, 480, 32, SDL_PIXELFORMAT_ARGB8888);
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
