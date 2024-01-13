#include <stdbool.h>

#include <SDL.h>

#include "audio.h"
#include "interpreter.h"
#include "vm.h"

const int FRAMES_PER_SECOND = 60;
const int CYCLES_PER_FRAME = 500 / FRAMES_PER_SECOND;

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    Chip8Context *chip8Context = NULL;

    if (init(&window, &renderer, &texture, &chip8Context) == -1) {
        kill(window, renderer, texture, chip8Context);
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
    kill(window, renderer, texture, chip8Context);

    return 0;
}