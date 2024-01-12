#ifndef CHIP8_INTERPRETER_H
#define CHIP8_INTERPRETER_H

#include <stdbool.h>

#include <SDL.h>

#include "vm.h"

// Initialises SDL and Chip8Context
int init(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture, Chip8Context **chip8Context);

// Handles SDL events
void pollEvents(SDL_Event *event, bool *quit, bool *keyState);

// Updates and draws to window
void updateScreen(SDL_Renderer *renderer, SDL_Texture *texture, bool display[32][64]);

// Draws rects for each enabled pixel
void drawRects(SDL_Renderer *renderer, bool display[32][64]);

// Destroys SDL components and frees memory taken up by Chip8Context
void kill(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, Chip8Context *chip8Context);

#endif
