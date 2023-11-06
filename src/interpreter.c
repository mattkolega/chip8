#include <time.h>

#include "interpreter.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;

void init(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture, Chip8Context **chip8Context, char *romFilename) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not be initialised! SDL_ErrorL %s\n", SDL_GetError());
    }

    *window = SDL_CreateWindow( "Chip-8 Interpreter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (*window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (*renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (*texture == NULL) {
        fprintf(stderr, "Texture could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    srand(time(NULL));  // Set seed for random number generator

    initContext(chip8Context, romFilename);
}

void pollEvents(SDL_Event *event, bool *quit) {
    while (SDL_PollEvent(event)) {
        switch(event->type) {
            case SDL_QUIT:
                *quit = true;
                break;
        }
    }
}

void updateScreen(SDL_Renderer *renderer, SDL_Texture *texture, bool display[32][64]) {
    // Render to texture
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render rects to texture
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    drawRects(renderer, display);

    // Render to window
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void drawRects(SDL_Renderer *renderer, bool display[32][64]) {
    SDL_Rect rectBuffer[64 * 32];
    int rectCount = 0;

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            if (display[i][j] == true) {
                SDL_Rect rect;
                rect.x = j * (SCREEN_WIDTH / 64);
                rect.y = i * (SCREEN_HEIGHT / 32);
                rect.w = SCREEN_WIDTH / 64;
                rect.h = SCREEN_HEIGHT / 32;
                rectBuffer[rectCount] = rect;
                rectCount++;
            }
        }
    }

    if (rectCount == 0) return;

    SDL_RenderFillRects(renderer, rectBuffer, rectCount);
}

void kill(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, Chip8Context *chip8Context) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free(chip8Context);

    SDL_Quit();
}