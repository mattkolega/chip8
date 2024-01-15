#ifndef CHIP8_APPLICATION_H
#define CHIP8_APPLICATION_H

#include <stdbool.h>

#include <SDL.h>

#include "vm.h"

// Initialises SDL
int initSDL(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture);

// Handles SDL events
void pollEvents(SDL_Event *event, bool *quit, bool *keyState);

// Updates and draws to window
void updateScreen(SDL_Renderer *renderer, SDL_Texture *texture, bool display[32][64]);

// Draws a rect for each enabled pixel
void drawRects(SDL_Renderer *renderer, bool display[32][64]);

// Destroys SDL components
void closeSDL(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);

#endif