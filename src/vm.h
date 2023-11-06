#ifndef CHIP8_VM_H
#define CHIP8_VM_H

#include <stdbool.h>
#include <stdint.h>

// Contains all components of a CHIP-8 system that need to be emulated
typedef struct Chip8Context {
    uint8_t memory[4096];  // 4KB of RAM
    bool display[32][64];  // Display is 64 pixels wide and 32 pixels high
    uint16_t stack[16];
    uint8_t sp;  // Stack pointer
    uint16_t pc;  // Program counter
    uint16_t index;  // Index register
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t v[16];  // Variable registers
} Chip8Context;

// Sets up chip8Context
void initContext(Chip8Context **chip8Context, char *fileName);

// Loads ROM file into CHIP-8 memory
void loadRom(char *filename, uint8_t *memory);

// Performs one cycle of the CHIP-8 fetch-decode-execute cycle
void fetchExecuteCycle(Chip8Context **chip8Context);

// Clears the display
void Op_00E0(bool display[][64]);

// Sets program counter to address NNN
void Op_1NNN(uint16_t *pc, uint16_t address);

// Sets register VX to value NN
void Op_6XNN(uint8_t *v, uint8_t registerIndex, uint8_t value);

// Adds value NN to the value currently in register VX
void Op_7XNN(uint8_t *v, uint8_t registerIndex, uint8_t value);

// Sets index register to address NN
void Op_ANNN(uint16_t *index, uint16_t address);

// Generates random number
void Op_CXNN(uint8_t *v, uint8_t registerIndex, uint8_t value);

// Draws n-width sprite to screen
void Op_DXYN(Chip8Context **chip8Context, uint16_t instruction);

#endif