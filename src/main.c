#include <stdbool.h>

#include <SDL.h>

#include "interpreter.h"
#include "vm.h"

const int FRAMES_PER_SECOND = 60;
const int CYCLES_PER_FRAME = 500 / FRAMES_PER_SECOND;

int main(int argc, char *argv[]) {
    char *romFilename = NULL;

    if (argc == 2) {
        romFilename = argv[1];
    } else {
        printf("ROM filename is expected as an argument.\n");
        return EXIT_FAILURE;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    Chip8Context *chip8Context = NULL;

    init(&window, &renderer, &texture, &chip8Context, romFilename);

    SDL_Event evt;
    bool quit = false;

    while (!quit) {
        uint64_t start = SDL_GetPerformanceCounter();

        pollEvents(&evt, &quit, chip8Context->keyState);

        for (int i = 0; i < CYCLES_PER_FRAME; i++)  // Run fetchExecuteCycle roughly 500 times per second
            fetchExecuteCycle(&chip8Context);

        if (chip8Context->delayTimer > 0)
            (chip8Context->delayTimer)--;
        if (chip8Context->soundTimer > 0)
            (chip8Context->soundTimer)--;

        updateScreen(renderer, texture, chip8Context->display);

        uint64_t end = SDL_GetPerformanceCounter();

        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;  // Calculate frame time
        float frameDelay = floorf(16.666f - elapsedMS);

        if (frameDelay > 0) {  // Cap FPS
            SDL_Delay(frameDelay);
        }
    }

    kill(window, renderer, texture, chip8Context);

    return EXIT_SUCCESS;
}