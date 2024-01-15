#include <time.h>

#include "application.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;

const int FOREGROUND_COLOURS[6*3] = {
    255, 255, 255,  // White
    255, 162, 150,  // Red
    111, 190, 138,  // Green
    105, 137, 170,  // Blue
    255, 220, 150,  // Yellow
    195, 115, 171   // Pink
};

const int BACKGROUND_COLOURS[6*3] = {
    0, 0, 0,       // White
    189, 69, 53,   // Red
    39, 138, 73,   // Green
    41, 81, 123,   // Blue
    189, 144, 53,  // Yellow
    143, 40, 112   // Pink
};

int colourIndex = 0;  // Used for choosing display colour

int initSDL(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Fatal Error! SDL could not be initialised! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow( "Chip-8 Interpreter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (*window == NULL) {
        SDL_Log("Fatal Error! Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (*renderer == NULL) {
        SDL_Log("Fatal Error! Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (*texture == NULL) {
        SDL_Log("Fatal Error! Screen Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    srand(time(NULL));  // Set seed for random number generator
}

void pollEvents(SDL_Event *event, bool *quit, bool *keyState) {
    while (SDL_PollEvent(event)) {
        switch(event->type) {
            case SDL_QUIT:
                *quit = true;
                break;
            case SDL_KEYDOWN:
                switch(event->key.keysym.scancode) {
                    case SDL_SCANCODE_1:
                        keyState[0x1] = true;
                        break;
                    case SDL_SCANCODE_2:
                        keyState[0x2] = true;
                        break;
                    case SDL_SCANCODE_3:
                        keyState[0x3] = true;
                        break;
                    case SDL_SCANCODE_4:
                        keyState[0xC] = true;
                        break;
                    case SDL_SCANCODE_Q:
                        keyState[0x4] = true;
                        break;
                    case SDL_SCANCODE_W:
                        keyState[0x5] = true;
                        break;
                    case SDL_SCANCODE_E:
                        keyState[0x6] = true;
                        break;
                    case SDL_SCANCODE_R:
                        keyState[0xD] = true;
                        break;
                    case SDL_SCANCODE_A:
                        keyState[0x7] = true;
                        break;
                    case SDL_SCANCODE_S:
                        keyState[0x8] = true;
                        break;
                    case SDL_SCANCODE_D:
                        keyState[0x9] = true;
                        break;
                    case SDL_SCANCODE_F:
                        keyState[0xE] = true;
                        break;
                    case SDL_SCANCODE_Z:
                        keyState[0xA] = true;
                        break;
                    case SDL_SCANCODE_X:
                        keyState[0x0] = true;
                        break;
                    case SDL_SCANCODE_C:
                        keyState[0xB] = true;
                        break;
                    case SDL_SCANCODE_V:
                        keyState[0xF] = true;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event->key.keysym.scancode) {
                    case SDL_SCANCODE_1:
                        keyState[0x1] = false;
                        break;
                    case SDL_SCANCODE_2:
                        keyState[0x2] = false;
                        break;
                    case SDL_SCANCODE_3:
                        keyState[0x3] = false;
                        break;
                    case SDL_SCANCODE_4:
                        keyState[0xC] = false;
                        break;
                    case SDL_SCANCODE_Q:
                        keyState[0x4] = false;
                        break;
                    case SDL_SCANCODE_W:
                        keyState[0x5] = false;
                        break;
                    case SDL_SCANCODE_E:
                        keyState[0x6] = false;
                        break;
                    case SDL_SCANCODE_R:
                        keyState[0xD] = false;
                        break;
                    case SDL_SCANCODE_A:
                        keyState[0x7] = false;
                        break;
                    case SDL_SCANCODE_S:
                        keyState[0x8] = false;
                        break;
                    case SDL_SCANCODE_D:
                        keyState[0x9] = false;
                        break;
                    case SDL_SCANCODE_F:
                        keyState[0xE] = false;
                        break;
                    case SDL_SCANCODE_Z:
                        keyState[0xA] = false;
                        break;
                    case SDL_SCANCODE_X:
                        keyState[0x0] = false;
                        break;
                    case SDL_SCANCODE_C:
                        keyState[0xB] = false;
                        break;
                    case SDL_SCANCODE_V:
                        keyState[0xF] = false;
                        break;
                    case SDL_SCANCODE_LEFT:
                        if (colourIndex == 0) {
                            colourIndex = 15;
                        } else {
                            colourIndex -= 3;  // Subtract 3 because each colour consists of 3 ints (R, G, B)
                        }
                        break;
                    case SDL_SCANCODE_RIGHT:
                        if (colourIndex == 15) {
                            colourIndex = 0;
                        } else {
                            colourIndex += 3;  // Add 3 because each colour consists of 3 ints (R, G, B)
                        }
                        break;
                }
                break;
        }
    }
}

void updateScreen(SDL_Renderer *renderer, SDL_Texture *texture, bool display[32][64]) {
    // Render to texture
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOURS[colourIndex], BACKGROUND_COLOURS[colourIndex+1], BACKGROUND_COLOURS[colourIndex+2], 255);
    SDL_RenderClear(renderer);

    // Render rects to texture
    SDL_SetRenderDrawColor(renderer, FOREGROUND_COLOURS[colourIndex], FOREGROUND_COLOURS[colourIndex+1], FOREGROUND_COLOURS[colourIndex+2], 255);
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

void closeSDL(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}