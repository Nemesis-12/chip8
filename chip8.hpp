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
    BYTE keypad[16];                // 8-bit keypad 
    BYTE graphics[32][64];          // Graphics emulation with 2048 elements

public:
    Chip8();
    void Load(char const* filename);
    void Emulate();

    // Getter and setter for memory
    BYTE getMemory(int index) const { return memory[index]; }
    void setMemory(int index, BYTE value) { memory[index] = value; }

    // Getter and setter for register
    BYTE getV(int index) const { return V[index]; }
    void setV(int index, BYTE value) { V[index] = value; }

    // Getter and setter for address register
    WORD getI() const { return I; }
    void setI(WORD value) { I = value; }

    // Getter and setter for program counter
    WORD getPC() const { return pc; }
    void setPC(WORD value) { pc = value; }

    // Getter and setter for opcode
    WORD getOpcode() const { return opcode; }
    void setOpcode(WORD value) { opcode = value; }

    // Getter and setter for stack
    WORD getStack(int index) const { return stack[index]; }
    void setStack(int index, WORD value) { stack[index] = value; }

    // Getter and setter for delay timer
    BYTE getDelayTimer() const { return delay_timer; }
    void setDelayTimer(BYTE value) { delay_timer = value; }

    // Getter and setter for sound timer
    BYTE getSoundTimer() const { return sound_timer; }
    void setSoundTimer(BYTE value) { sound_timer = value; }

    // Getter and setter for stack pointer
    BYTE getPtr() const { return ptr; }
    void setPtr(BYTE value) { ptr = value; }

    // Getter and setter for keypad
    BYTE getKeypad(int index) const { return keypad[index]; }
    void setKeypad(int index, BYTE value) { keypad[index] = value; }

    // Getter and setter for graphics
    BYTE getGraphics(int y, int x) const { return graphics[y][x]; }
    void setGraphics(int y, int x, BYTE value) { graphics[y][x] = value; }
};