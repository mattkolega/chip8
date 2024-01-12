#ifndef CHIP8_VM_H
#define CHIP8_VM_H

#include <stdbool.h>
#include <stdint.h>

// Contains all components of a CHIP-8 system that need to be emulated
typedef struct Chip8Context {
    uint8_t memory[4096];  // 4KB of RAM
    bool display[32][64];  // Display is 64 pixels wide and 32 pixels high
    uint16_t stack[16];
    uint8_t sp;  // Stack pointer - points to first available stack location
    uint16_t pc;  // Program counter
    uint16_t index;  // Index register
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t v[16];  // Variable registers
    bool keyState[16];  // Tracks whether keys corresponding to hex chars are pressed or not
    bool previousKeyState[16];  // Used to track key press and release for FX0A
} Chip8Context;

// Sets up chip8Context
int initContext(Chip8Context **chip8Context);

// Loads ROM file into CHIP-8 memory
int loadRom(uint8_t *memory);

// Loads font data into CHIP-8 memory
void loadFontData(uint8_t *memory);

// Performs one cycle of the CHIP-8 fetch-decode-execute cycle
void fetchExecuteCycle(Chip8Context **chip8Context);

#endif