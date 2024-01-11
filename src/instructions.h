#ifndef CHIP_8_INSTRUCTIONS_H
#define CHIP_8_INSTRUCTIONS_H

#include <stdbool.h>
#include <stdint.h>

#include "vm.h"

/*
 *  -- Opcodes --
 */

// Clears the display
void op_00E0(bool display[][64]);

// Returns from subroutine
void op_00EE(uint16_t *pc, uint16_t *stack, uint8_t *sp);

// Sets program counter to address NNN
void op_1NNN(uint16_t *pc, uint16_t address);

// Calls subroutine
void op_2NNN(uint16_t *pc, uint16_t *stack, uint8_t *sp, uint16_t address);

// Skips instruction if VX and NN are equal
void op_3XNN(uint16_t *pc, uint8_t *v, uint8_t registerIndex, uint8_t value);

// Skips instruction if VX and NN aren't equal
void op_4XNN(uint16_t *pc, uint8_t *v, uint8_t registerIndex, uint8_t value);

// Skips instruction if VX and VY are equal
void op_5XY0(uint16_t *pc, uint8_t *v, uint16_t instruction);

// Sets register VX to value NN
void op_6XNN(uint8_t *v, uint8_t registerIndex, uint8_t value);

// Adds value NN to the value currently in register VX
void op_7XNN(uint8_t *v, uint8_t registerIndex, uint8_t value);

// Sets VX to value of VY
void op_8XY0(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to result of binary OR of VX and VY
void op_8XY1(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to result of binary AND of VX and VY
void op_8XY2(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to result of binary XOR of VX and VY
void op_8XY3(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to sum of VX and VY
void op_8XY4(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to VX minus VY
void op_8XY5(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to VY and shifts VX 1 bit to the right
void op_8XY6(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to VY minus VX
void op_8XY7(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Sets VX to VY and shifts VX 1 bit to the left
void op_8XYE(uint8_t *v, int xRegisterIndex, int yRegisterIndex);

// Skips instruction if VX and VY aren't equal
void op_9XY0(uint16_t *pc, uint8_t *v, uint16_t instruction);

// Sets index register to address NN
void op_ANNN(uint16_t *index, uint16_t address);

// Jumps to address NNN plus value in V0
void op_BNNN(uint16_t *pc, uint8_t *v, uint16_t address);

// Generates random number
void op_CXNN(uint8_t *v, uint8_t registerIndex, uint8_t value);

// Draws n-width sprite to screen
void op_DXYN(Chip8Context **chip8Context, uint16_t instruction);

// Skips one instruction if key equal to VX value is pressed
void op_EX9E(Chip8Context **chip8Context, uint8_t registerIndex);

// Skips one instruction if key equal to VX value is not pressed
void op_EXA1(Chip8Context **chip8Context, uint8_t registerIndex);

// Sets VX to delayTimer value
void op_FX07(uint8_t *v, uint8_t *delayTimer, uint8_t registerIndex);

// Waits for key to be pressed and adds keyvalue to VX
void op_FX0A(Chip8Context **chip8Context, uint8_t registerIndex);

// Sets delayTimer to VX value
void op_FX15(uint8_t *v, uint8_t *delayTimer, uint8_t registerIndex);

// Sets soundTimer to VX value
void op_FX18(uint8_t *v, uint8_t *soundTimer, uint8_t registerIndex);

// Adds value in VX to index register
void op_FX1E(uint8_t *v, uint16_t *index, uint8_t registerIndex);

// Sets index register to address corresponding to a hexadecimal character
void op_FX29(uint8_t *v, uint16_t *index, uint8_t registerIndex);

// Converts VX value to binary-coded decimal and store result at address I, I+1, I+2
void op_FX33(Chip8Context **chip8Context, uint8_t registerIndex);

// Writes registers to memory
void op_FX55(Chip8Context **chip8Context, uint8_t registerIndex);

// Loads registers from memory
void op_FX65(Chip8Context **chip8Context, uint8_t registerIndex);

#endif