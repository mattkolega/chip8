#include <intrin.h>
#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

void initContext(Chip8Context **chip8Context, char* romFilename) {
    *chip8Context = calloc(1, sizeof(Chip8Context));
    loadRom(romFilename, (*chip8Context)->memory);
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

void fetchExecuteCycle(Chip8Context **chip8Context) {
    // Fetch
    uint8_t nibbleOne = (*chip8Context)->memory[(*chip8Context)->pc];
    uint8_t nibbleTwo = (*chip8Context)->memory[(*chip8Context)->pc + 1];
    uint16_t instruction = nibbleOne << 8 | nibbleTwo;
    (*chip8Context)->pc += 2;

    // Decode and Execute
    switch ((instruction & 0xF000) >> 12) {
        case 0x0:
            switch (instruction & 0x00FF) {
                case 0xE0:
                    Op_00E0((*chip8Context)->display);
                    break;
                case 0xEE:
                    break;
            }
            break;
        case 0x1:
            Op_1NNN(&(*chip8Context)->pc, (instruction & 0x0FFF));
            break;
        case 0x6:
            Op_6XNN((*chip8Context)->v, ((instruction & 0x0F00) >> 8), (instruction & 0x00FF));
            break;
        case 0x7:
            Op_7XNN((*chip8Context)->v, ((instruction & 0x0F00) >> 8), (instruction & 0x00FF));
            break;
        case 0xA:
            Op_ANNN(&(*chip8Context)->index, (instruction & 0x0FFF));
            break;
        case 0xC:
            Op_CXNN((*chip8Context)->v, ((instruction & 0x0F00) >> 8), (instruction & 0x00FF));
            break;
        case 0xD:
            Op_DXYN(chip8Context, instruction);
            break;
    }
}

void Op_00E0(bool display[][64]) {
    memset(display, 0, (32 * 64) * sizeof(bool));
}

void Op_1NNN(uint16_t *pc, uint16_t address) {
    *pc = address;
}

void Op_6XNN(uint8_t *v, uint8_t registerIndex, uint8_t value) {
    v[registerIndex] = value;
}

void Op_7XNN(uint8_t *v, uint8_t registerIndex, uint8_t value) {
    v[registerIndex] += value;
}

void Op_ANNN(uint16_t *index, uint16_t address) {
    *index = address;
}

void Op_CXNN(uint8_t *v, uint8_t registerIndex, uint8_t value) {
    int randNum = rand();
    int result = randNum & value;  // binary AND randNum with NN value

    v[registerIndex] = result;
}

void Op_DXYN(Chip8Context **chip8Context, uint16_t instruction) {
    int xIndex = (instruction & 0x0F00) >> 8;
    int yIndex = (instruction & 0x00F0) >> 4;

    int xCoord = (*chip8Context)->v[xIndex] % 64;  // Make starting xCoord wrap around
    int yCoord = (*chip8Context)->v[yIndex] % 32;  // Make starting yCoord wrap around

    int spriteRows = instruction & 0x000F;  // Get number of rows to draw for sprite

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

            if (spriteBit ^ (*chip8Context)->display[currentYCoord][currentXCoord]) {
                (*chip8Context)->display[currentYCoord][currentXCoord] = true;
            } else {
                (*chip8Context)->display[currentYCoord][currentXCoord] = false;
                (*chip8Context)->v[0xF] = 1;  // Set VF register to 1
            }
        }
    }
}