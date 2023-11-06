#include <stdbool.h>

#include <SDL.h>

#include "interpreter.h"
#include "vm.h"

const int CYCLES_PER_FRAME = 500 / 60;

int main(int argc, char *argv[]) {
    char* romFilename = NULL;

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

        pollEvents(&evt, &quit);
        for (int i = 0; i < CYCLES_PER_FRAME; i++) {  // Run fetchExecuteCycle roughly 500 times per second
            fetchExecuteCycle(&chip8Context);
        }
        updateScreen(renderer, texture, chip8Context->display);

        uint64_t end = SDL_GetPerformanceCounter();

        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;  // Calculate frame time
        float delayTime = floorf(16.666f - elapsedMS);

        if (delayTime > 0) {  // Cap FPS to 60
            SDL_Delay(delayTime);
        }
    }

    kill(window, renderer, texture, chip8Context);

    return EXIT_SUCCESS;
}