#include "vm.h"

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include "tinyfiledialogs.h"

#include "instructions.h"
#include "utils.h"

int initContext(Chip8Context **chip8Context) {
    *chip8Context = calloc(1, sizeof(Chip8Context));
    if (loadRom((*chip8Context)->memory) == -1) return -1;
    loadFontData((*chip8Context)->memory);
    (*chip8Context)->pc = 512;
}

int loadRom(uint8_t *memory) {
    char const *filterPatterns[1]={"*.ch8"};  // Only display .ch8 files in file dialog

    FILE *fptr;

    while (true) {
        const char *filename = tinyfd_openFileDialog(
                "Choose CHIP-8 Rom",  // Dialog title
                "./",                 // Default path
                1,                    // Num of filter patterns
                filterPatterns,       // Filter patterns
                ".ch8" ,              // Filter description
                0                     // Allow multiple files
        );

        if (filename == NULL) {
            SDL_Log("File dialog closed. Program execution is now aborting.");
            return -1;
        }

        fptr = fopen(filename, "rb");

        if (fptr == NULL) {
            SDL_Log("Error opening Chip-8 rom. Please try again.");
        } else {
            break;  // Break loop if rom file has successfully opened
        }
    }

    fseek(fptr, 0l, SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);

    unsigned char *buffer = malloc(fileSize);

    fread(buffer, sizeof(char), fileSize, fptr);

    size_t memoryIndex = 512;

    for (size_t i = 0; i < fileSize; i++) {
        if (i > 4095) {  // Prevent memory array out of bounds error
            SDL_Log("Fatal Error! ROM filesize is too large to fit in CHIP8 memory.");
            free(buffer);
            return -1;
        }
        memory[memoryIndex] = buffer[i];
        memoryIndex++;
    }

    free(buffer);
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