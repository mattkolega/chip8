#include <stdbool.h>

#include <SDL.h>

#include "application.h"
#include "audio.h"
#include "vm.h"

#define FRAMES_PER_SECOND 60
#define CYCLES_PER_FRAME (500 / FRAMES_PER_SECOND)

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    if (initSDL(&window, &renderer, &texture) == -1) {
        closeSDL(window, renderer, texture);
        return -1;
    }

    Chip8Context *chip8Context = NULL;

    if (initContext(&chip8Context) == -1) {
        free(chip8Context);
        closeSDL(window, renderer, texture);
        return -1;
    }

    AudioContext audioContext = initAudio();

    SDL_Event evt;
    bool quit = false;

    while (!quit) {
        uint64_t start = SDL_GetPerformanceCounter();

        pollEvents(&evt, &quit, chip8Context->keyState);

        for (int i = 0; i < CYCLES_PER_FRAME; i++)  // Run fetchExecuteCycle roughly 500 times per second
            fetchExecuteCycle(&chip8Context);

        if (chip8Context->delayTimer > 0)
            (chip8Context->delayTimer)--;

        if (chip8Context->soundTimer > 0) {
            (chip8Context->soundTimer)--;
            startPlayback(audioContext.device);  // Play sound as long as timer is above 0
        } else {
            stopPlayback(audioContext.device);
        }

        updateScreen(renderer, texture, chip8Context->display);

        uint64_t end = SDL_GetPerformanceCounter();

        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;  // Calculate frame time
        float frameDelay = floorf(16.666f - elapsedMS);

        if (frameDelay > 0) {  // Cap FPS
            SDL_Delay(frameDelay);
        }
    }

    destroyAudio(audioContext);
    free(chip8Context);
    closeSDL(window, renderer, texture);

    return 0;
}