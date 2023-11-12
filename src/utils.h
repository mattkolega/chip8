#ifndef CHIP8_UTILS_H
#define CHIP8_UTILS_H

#include <stdint.h>

/*
 * -- Instruction Decode Helpers --
 * A 'nibble' is a half-byte component of an instruction
 */

uint8_t getFirstNibble(uint16_t instruction);

uint8_t getSecondNibble(uint16_t instruction);

uint8_t getThirdNibble(uint16_t instruction);

uint8_t getFourthNibble(uint16_t instruction);

uint8_t getLastHalfInstruct(uint16_t instruction);

uint16_t getLastThreeNibbles(uint16_t instruction);

#endif