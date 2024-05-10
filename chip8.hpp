#pragma once
#include <cstdint>
#include <vector>

class Chip8 {
private:
    // Basic architecture of CHIP-8 system
    typedef uint8_t BYTE;     // 8-bits
    typedef uint16_t WORD;    // 16-bits

    BYTE memory[4096];              // Memory size of CHIP-8 system
    BYTE V[16];                     // 16 8-bit registers 
    WORD I;                         // 16-bit address register
    WORD pc;                        // 16-bit program counter
    WORD opcode;                    // 16-bit operation code
    WORD stack[16];                 // 16-bit stack
    BYTE delay_timer;               // 8-bit delay timer
    BYTE sound_timer;               // 8-bit sound timer
    BYTE ptr;                       // 8-bit stack pointer

public:
    Chip8();
    void Load(char const* filename);
    void Emulate();

    BYTE graphics[64 * 32];          // Graphics emulation with 2048 elements
};