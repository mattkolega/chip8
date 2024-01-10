#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "vm.h"
#include "utils.h"

void initContext(Chip8Context **chip8Context, char* romFilename) {
    *chip8Context = calloc(1, sizeof(Chip8Context));
    loadRom(romFilename, (*chip8Context)->memory);
    loadFontData((*chip8Context)->memory);
    (*chip8Context)->pc = 512;
}

void loadRom(char *filename, uint8_t *memory) {
    FILE *fptr;

    fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        printf("Error opening Chip-8 rom.");
        exit(EXIT_FAILURE);
    }

    fseek(fptr, 0l, SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    unsigned char buffer[fileSize];

    fread(buffer, sizeof(char), fileSize, fptr);

    size_t memoryIndex = 512;

    for (size_t i = 0; i < fileSize; i++) {
        memory[memoryIndex] = buffer[i];
        memoryIndex++;
    }
}

void loadFontData(uint8_t *memory) {
    uint8_t fontData[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };

    for (int i = 0; i < 80; i++)
        memory[0x050 + i] = fontData[i];
}

void fetchExecuteCycle(Chip8Context **chip8Context) {
    // Fetch
    uint8_t byteOne = (*chip8Context)->memory[(*chip8Context)->pc];
    uint8_t byteTwo = (*chip8Context)->memory[(*chip8Context)->pc + 1];
    uint16_t instruction = byteOne << 8 | byteTwo;
    (*chip8Context)->pc += 2;

    // Decode and Execute
    switch (getFirstNibble(instruction)) {  // Check first nibble of instruction
        case 0x0:
            switch (getLastHalfInstruct(instruction)) {  // Check second half of instruction
                case 0xE0:
                    op_00E0((*chip8Context)->display);
                    break;
                case 0xEE:
                    op_00EE(&(*chip8Context)->pc, (*chip8Context)->stack, &(*chip8Context)->sp);
                    break;
            }
            break;
        case 0x1:
            op_1NNN(&(*chip8Context)->pc, getLastThreeNibbles(instruction));
            break;
        case 0x2:
            op_2NNN(&(*chip8Context)->pc, (*chip8Context)->stack, &(*chip8Context)->sp, getLastThreeNibbles(instruction));
            break;
        case 0x3:
            op_3XNN(&(*chip8Context)->pc, (*chip8Context)->v, getSecondNibble(instruction), getLastHalfInstruct(instruction));
            break;
        case 0x4:
            op_4XNN(&(*chip8Context)->pc, (*chip8Context)->v, getSecondNibble(instruction), getLastHalfInstruct(instruction));
            break;
        case 0x5:
            op_5XY0(&(*chip8Context)->pc, (*chip8Context)->v, instruction);
            break;
        case 0x6:
            op_6XNN((*chip8Context)->v, getSecondNibble(instruction), getLastHalfInstruct(instruction));
            break;
        case 0x7:
            op_7XNN((*chip8Context)->v, getSecondNibble(instruction), getLastHalfInstruct(instruction));
            break;
        case 0x8:
            switch (getFourthNibble(instruction)) {  // Check last nibble of instruction
                case 0x0:
                    op_8XY0((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0x1:
                    op_8XY1((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0x2:
                    op_8XY2((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0x3:
                    op_8XY3((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0x4:
                    op_8XY4((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0x5:
                    op_8XY5((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0x6:
                    op_8XY6((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0x7:
                    op_8XY7((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
                case 0xE:
                    op_8XYE((*chip8Context)->v, getSecondNibble(instruction), getThirdNibble(instruction));
                    break;
            }
            break;
        case 0x9:
            op_9XY0(&(*chip8Context)->pc, (*chip8Context)->v, instruction);
            break;
        case 0xA:
            op_ANNN(&(*chip8Context)->index, getLastThreeNibbles(instruction));
            break;
        case 0xB:
            op_BNNN(&(*chip8Context)->pc, (*chip8Context)->v, getLastThreeNibbles(instruction));
            break;
        case 0xC:
            op_CXNN((*chip8Context)->v, getSecondNibble(instruction), getLastHalfInstruct(instruction));
            break;
        case 0xD:
            op_DXYN(chip8Context, instruction);
            break;
        case 0xE:
            switch (getLastHalfInstruct(instruction)) {
                case 0x9E:
                    op_EX9E(chip8Context, getSecondNibble(instruction));
                    break;
                case 0xA1:
                    op_EXA1(chip8Context, getSecondNibble(instruction));
                    break;
            }
            break;
        case 0xF:
            switch (getThirdNibble(instruction)) {
                case 0x0:
                    switch (getFourthNibble(instruction)) {
                        case 0x7:
                            op_FX07((*chip8Context)->v, &(*chip8Context)->delayTimer, getSecondNibble(instruction));
                            break;
                        case 0xA:
                            op_FX0A(chip8Context, getSecondNibble(instruction));
                            break;
                    }
                    break;
                case 0x1:
                    switch (getFourthNibble(instruction)) {
                        case 0x5:
                            op_FX15((*chip8Context)->v, &(*chip8Context)->delayTimer, getSecondNibble(instruction));
                            break;
                        case 0x8:
                            op_FX18((*chip8Context)->v, &(*chip8Context)->soundTimer, getSecondNibble(instruction));
                            break;
                        case 0xE:
                            op_FX1E((*chip8Context)->v, &(*chip8Context)->index, getSecondNibble(instruction));
                            break;
                    }
                    break;
                case 0x2:
                    op_FX29((*chip8Context)->v, &(*chip8Context)->index, getSecondNibble(instruction));
                    break;
                case 0x3:
                    op_FX33(chip8Context, getSecondNibble(instruction));
                    break;
                case 0x5:
                    op_FX55(chip8Context, getSecondNibble(instruction));
                    break;
                case 0x6:
                    op_FX65(chip8Context, getSecondNibble(instruction));
                    break;
            }
            break;
    }
}

void op_00E0(bool display[][64]) {
    memset(display, 0, (32 * 64) * sizeof(bool));
}

void op_00EE(uint16_t *pc, uint16_t *stack, uint8_t *sp) {
    (*sp)--;
    *pc = stack[*sp];
}

void op_1NNN(uint16_t *pc, uint16_t address) {
    *pc = address;
}

void op_2NNN(uint16_t *pc, uint16_t *stack, uint8_t *sp, uint16_t address) {
    stack[*sp] = *pc;
    (*sp)++;
    *pc = address;
}

void op_3XNN(uint16_t *pc, uint8_t *v, uint8_t registerIndex, uint8_t value) {
    if (v[registerIndex] == value)
        *pc += 2;
}

void op_4XNN(uint16_t *pc, uint8_t *v, uint8_t registerIndex, uint8_t value) {
    if (v[registerIndex] != value)
        *pc += 2;
}

void op_5XY0(uint16_t *pc, uint8_t *v, uint16_t instruction) {
    int xIndex = getSecondNibble(instruction);
    int yIndex = getThirdNibble(instruction);

    if (v[xIndex] == v[yIndex])
        *pc += 2;
}

void op_6XNN(uint8_t *v, uint8_t registerIndex, uint8_t value) {
    v[registerIndex] = value;
}

void op_7XNN(uint8_t *v, uint8_t registerIndex, uint8_t value) {
    v[registerIndex] += value;
}

void op_8XY0(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    v[xRegisterIndex] = v[yRegisterIndex];
}

void op_8XY1(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    v[xRegisterIndex] |= v[yRegisterIndex];
    v[0xF] = 0;
}

void op_8XY2(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    v[xRegisterIndex] &= v[yRegisterIndex];
    v[0xF] = 0;
}

void op_8XY3(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    v[xRegisterIndex] ^= v[yRegisterIndex];
    v[0xF] = 0;
}

void op_8XY4(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    uint16_t sum = v[xRegisterIndex] + v[yRegisterIndex];
    v[xRegisterIndex] = sum & 0xFF;
    if (sum > 255)  // Test for overflow
        v[0xF] = 1;
    else
        v[0xF] = 0;
}

void op_8XY5(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    uint8_t vF;

    if (v[xRegisterIndex] > v[yRegisterIndex])
        vF = 1;
    else
        vF = 0;

    v[xRegisterIndex] -= v[yRegisterIndex];
    v[0xF] = vF;
}

void op_8XY6(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    v[xRegisterIndex] = v[yRegisterIndex];
    uint8_t vF = (v[xRegisterIndex] & 0b00000001);  // Set VF to value of bit to be shifted
    v[xRegisterIndex] >>= 1;
    v[0xF] = vF;
}

void op_8XY7(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    uint8_t vF;

    if (v[yRegisterIndex] > v[xRegisterIndex])
        vF = 1;
    else
        vF = 0;

    v[xRegisterIndex] = v[yRegisterIndex] - v[xRegisterIndex];
    v[0xF] = vF;
}

void op_8XYE(uint8_t *v, int xRegisterIndex, int yRegisterIndex) {
    v[xRegisterIndex] = v[yRegisterIndex];
    uint8_t vF = (v[xRegisterIndex] & 0b10000000) >> 7;  // Set VF to value of bit to be shifted
    v[xRegisterIndex] <<= 1;
    v[0xF] = vF;
}

void op_9XY0(uint16_t *pc, uint8_t *v, uint16_t instruction) {
    int xIndex = getSecondNibble(instruction);
    int yIndex = getThirdNibble(instruction);

    if (v[xIndex] != v[yIndex])
        *pc += 2;
}

void op_ANNN(uint16_t *index, uint16_t address) {
    *index = address;
}

void op_BNNN(uint16_t *pc, uint8_t *v, uint16_t address) {
    *pc = address + v[0];
}

void op_CXNN(uint8_t *v, uint8_t registerIndex, uint8_t value) {
    int randNum = rand();
    int result = randNum & value;  // binary AND randNum with NN value

    v[registerIndex] = result;
}

void op_DXYN(Chip8Context **chip8Context, uint16_t instruction) {
    int xIndex = getSecondNibble(instruction);
    int yIndex = getThirdNibble(instruction);

    int xCoord = (*chip8Context)->v[xIndex] % 64;  // Make starting xCoord wrap around
    int yCoord = (*chip8Context)->v[yIndex] % 32;  // Make starting yCoord wrap around

    int spriteRows = getFourthNibble(instruction);  // Get number of rows to draw for sprite

    (*chip8Context)->v[0xF] = 0;  // Set VF register to 0

    for (int n = 0; n < spriteRows; n++) {
        int currentYCoord = yCoord + n;  // Increment yCoord for each row
        if (currentYCoord > 31) break;
        uint8_t spriteByte = (*chip8Context)->memory[(*chip8Context)->index + n];
        uint8_t bitmask = 0b10000000;
        uint8_t bitshiftAmount = 7;

        for (int i = 0; i < 8; i++) {
            int currentXCoord = xCoord + i;  // Increment xCoord for each column
            if (currentXCoord > 63) break;
            uint8_t spriteBit = (spriteByte & bitmask) >> bitshiftAmount;
            bitmask = bitmask >> 1;
            bitshiftAmount -= 1;

            if (spriteBit ^ (*chip8Context)->display[currentYCoord][currentXCoord]) {  // Binary XOR
                (*chip8Context)->display[currentYCoord][currentXCoord] = true;
            } else if (spriteBit & (*chip8Context)->display[currentYCoord][currentXCoord]) {  // Binary AND
                (*chip8Context)->display[currentYCoord][currentXCoord] = false;
                (*chip8Context)->v[0xF] = 1;
            }
        }
    }
}

void op_EX9E(Chip8Context **chip8Context, uint8_t registerIndex) {
    if ((*chip8Context)->keyState[(*chip8Context)->v[registerIndex]] == true) {
        (*chip8Context)->pc += 2;
    }
}

void op_EXA1(Chip8Context **chip8Context, uint8_t registerIndex) {
    if ((*chip8Context)->keyState[(*chip8Context)->v[registerIndex]] == false) {
        (*chip8Context)->pc += 2;
    }
}

void op_FX07(uint8_t *v, uint8_t *delayTimer, uint8_t registerIndex) {
    v[registerIndex] = *delayTimer;
}

void op_FX0A(Chip8Context **chip8Context, uint8_t registerIndex) {
    // First check if any keys were previously pressed and are now released
    for (int j = 0; j < 16; j++) {
        if ((*chip8Context)->previousKeyState[j] == true) {
            if ((*chip8Context)->keyState[j] == false) {
                (*chip8Context)->v[registerIndex] = j;
                memset((*chip8Context)->previousKeyState, 0, sizeof((*chip8Context)->previousKeyState));  // Clear array
                return;
            }
        }
    }

    memset((*chip8Context)->previousKeyState, 0, sizeof((*chip8Context)->previousKeyState));  // Clear array

    // If no keys were released, check for pressed keys to add to previousKeyState to be checked next cycle
    for (int i = 0; i < 16; i++) {
        if ((*chip8Context)->keyState[i] == true) {
            (*chip8Context)->previousKeyState[i] = true;
        }
    }

    (*chip8Context)->pc -= 2;  // De-increment PC to make block further instruction execution
}

void op_FX15(uint8_t *v, uint8_t *delayTimer, uint8_t registerIndex) {
    *delayTimer = v[registerIndex];
}

void op_FX18(uint8_t *v, uint8_t *soundTimer, uint8_t registerIndex) {
    *soundTimer = v[registerIndex];
}

void op_FX1E(uint8_t *v, uint16_t *index, uint8_t registerIndex) {
    *index += v[registerIndex];
}

void op_FX29(uint8_t *v, uint16_t *index, uint8_t registerIndex) {
    *index = 0x050 + (v[registerIndex] * 5);  // Font data is stored at address 0x050
}

void op_FX33(Chip8Context **chip8Context, uint8_t registerIndex) {
    uint8_t number = (*chip8Context)->v[registerIndex];

    uint8_t numberDigits[3];
    numberDigits[0] = (number / 100) % 10;
    numberDigits[1] = (number / 10) % 10;
    numberDigits[2] = number % 10;

    for (int i = 0; i < 3; i++) {
        (*chip8Context)->memory[(*chip8Context)->index + i] = numberDigits[i];
    }
}

void op_FX55(Chip8Context **chip8Context, uint8_t registerIndex) {
    for (int i = 0; i <= registerIndex; i++) {
        (*chip8Context)->memory[(*chip8Context)->index + i] = (*chip8Context)->v[i];
    }
    (*chip8Context)->index += registerIndex + 1;
}

void op_FX65(Chip8Context **chip8Context, uint8_t registerIndex) {
    for (int i = 0; i <= registerIndex; i++) {
        (*chip8Context)->v[i] = (*chip8Context)->memory[(*chip8Context)->index + i];
    }
    (*chip8Context)->index += registerIndex + 1;
}