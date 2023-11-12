#include "utils.h"

uint8_t getFirstNibble(uint16_t instruction) {
    return (instruction & 0xF000) >> 12;
}

uint8_t getSecondNibble(uint16_t instruction) {
    return (instruction & 0x0F00) >> 8;
}

uint8_t getThirdNibble(uint16_t instruction) {
    return (instruction & 0x00F0) >> 4;
}

uint8_t getFourthNibble(uint16_t instruction) {
    return instruction & 0x000F;
}

uint8_t getLastHalfInstruct(uint16_t instruction) {
    return instruction & 0x00FF;
}

uint16_t getLastThreeNibbles(uint16_t instruction) {
    return instruction & 0x0FFF;
}