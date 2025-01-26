// CHIP-8 Emulator
#include "chip8.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

// Font set of the emulator
uint8_t fontSet[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

// Initialize Chip8
Chip8::Chip8() {
    setPC(0x200);                 // Games are loaded at this location
    setI(0);                      // Reset address register
    
    // Set registers to zero
    for (int i = 0; i < 16; ++i) setV(i, 0);
    for (int i = 0; i < 4096; ++i) setMemory(i, 0);

    // Initialize graphics
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            setGraphics(y, x, 0);
        }
    }

    // Load font set onto memory
    for (int i = 0; i < 80; ++i) {
        setMemory(i, fontSet[i]);
    }

    srand(time(NULL));
}

void Chip8::Load(char const* filename) {
    // Read game onto memory as a binary file and move pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        // Get size and allocate buffer
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        // Fill buffer by going to the start of file
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        // Load game onto memory location 0x200
        for (auto i = 0; i < size; ++i) {
            setMemory(getPC() + i, buffer[i]);
        }

        delete[] buffer;
    }
}

void Chip8::Emulate() {
    // Fetch opcode
    // setOpcode(getMemory(getPC()) << 8 | getMemory(getPC() + 1));
    setPC(getPC() + 2);

    uint8_t Vx = (getOpcode() & 0x0F00) >> 8;
    uint8_t Vy = (getOpcode() & 0x00F0) >> 4;

    uint16_t sum = 0;
    uint8_t randByte = rand() % 256;

    // Decode opcode
    // Check first 4 bits and decide what to do
    switch (getOpcode() & 0xF000) {
        case 0x0000:
            // 0x00E0 - Clear screen
            if (getOpcode() == 0x00E0) {
                for (int y = 0; y < 32; ++y) {
                    for (int x = 0; x < 64; ++x) {
                        setGraphics(y, x, 0);
                    }
                }
            }

            // 0x00EE - Return from subroutine
            else if (getOpcode() == 0x00EE) {
                setPtr(getPtr() - 1);
                setPC(getStack(getPtr()));
            }
            break;

        // 0xANNN - Set I = NNN
        case 0xA000:
            setI(getOpcode() & 0x0FFF);
            break;

        // 0x1NNN - Set PC = NNN
        case 0x1000:
            setPC(getOpcode() & 0x0FFF);
            break;

        // 0x2NNN - Call subroutine at NNN
        case 0x2000:
            setStack(getPtr(), getPC());
            setPtr(getPtr() + 1);
            setPC(getOpcode() & 0x0FFF);
            break;

        // 0x3XKK - Skip next instruction if Vx == KK
        case 0x3000:
            if (getV(Vx) == (getOpcode() & 0x00FF)) {
                setPC(getPC() + 2);
            }
            break;

        // 0x4XKK - Skip next instruction if Vx != KK
        case 0x4000:
            if (getV(Vx) != (getOpcode() & 0x00FF)) {
                setPC(getPC() + 2);
            }
            break;

        // 0x5XY0 - Skip next instruction if Vx == Vy
        case 0x5000:
            if (getV(Vx) == getV(Vy)) {
                setPC(getPC() + 2);
            }
            break;

        // 0x6XKK - Set Vx = KK
        case 0x6000:
            setV(Vx, getOpcode() & 0x00FF);
            break;

        // 0x7XKK - Set Vx = Vx + KK
        case 0x7000:
            setV(Vx, getV(Vx) + (getOpcode() & 0x00FF));
            break;

        // 0x8000 instruction set
        case 0x8000: {
            switch (getOpcode() & 0x000F) {
                case 0x0000: // Set Vx = Vy
                    setV(Vx, getV(Vy));
                    break;

                case 0x0001: // Set Vx = Vx OR Vy
                    setV(Vx, getV(Vx) | getV(Vy));
                    break;

                case 0x0002: // Set Vx = Vx AND Vy
                    setV(Vx, getV(Vx) & getV(Vy));
                    break;

                case 0x0003: // Set Vx = Vx XOR Vy
                    setV(Vx, getV(Vx) ^ getV(Vy));
                    break;

                case 0x0004: // Set Vx = Vx + Vy, set VF = carry
                    sum = getV(Vx) + getV(Vy);
                    setV(0xF, sum > 255 ? 1 : 0);
                    setV(Vx, sum & 0xFF);
                    break;

                case 0x0005: // Set Vx = Vx - Vy, set VF = NOT borrow
                    setV(0xF, getV(Vx) > getV(Vy) ? 1 : 0);
                    setV(Vx, getV(Vx) - getV(Vy));
                    break;

                case 0x0006: // Set Vx = Vx >> 1, set VF = LSB
                    setV(0xF, getV(Vx) & 1);
                    setV(Vx, getV(Vx) >> 1);
                    break;

                case 0x0007: // Set Vx = Vy - Vx, set VF = NOT borrow
                    setV(0xF, getV(Vy) > getV(Vx) ? 1 : 0);
                    setV(Vx, getV(Vy) - getV(Vx));
                    break;

                case 0x000E: // Set Vx = Vx << 1, set VF = MSB
                    setV(0xF, getV(Vx) >> 7);
                    setV(Vx, getV(Vx) << 1);
                    break;
            }
            break;
        }

        // 0x9XY0 - Skip next instruction if Vx != Vy
        case 0x9000:
            if (getV(Vx) != getV(Vy)) {
                setPC(getPC() + 2);
            }
            break;

        // 0xBNNN - Jump to location NNN + V0
        case 0xB000:
            setPC(getV(0) + (getOpcode() & 0x0FFF));
            break;

        // 0xCXY0 - Set Vx = random byte AND KK
        case 0xC000:
            setV(Vx, (rand() % 256) & (getOpcode() & 0x00FF));
            break;

        // 0xDXYN - Display n-byte sprite at location (Vx, Vy) and set VF = collision
        case 0xD000: {
            uint8_t height = getOpcode() & 0x000F;
            uint8_t sprite;

            setV(0xF, 0); // Reset collision flag

            for (int i = 0; i < height; ++i) {
                sprite = getMemory(getI() + i);

                for (int j = 0; j < 8; ++j) {
                    if (sprite & (0x80 >> j)) {
                        int x = (getV(Vx) + j) % 64;
                        int y = (getV(Vy) + i) % 32;

                        if (getGraphics(y, x)) {
                            setV(0xF, 1); // Collision detected
                        }

                        setGraphics(y, x, getGraphics(y, x) ^ 1); // XOR the pixel
                    }
                }
            }
            setPC(getPC() + 2);
            break;
        }
    }

    // Decrement timers when set
    if (getDelayTimer() > 0) {
        setDelayTimer(getDelayTimer() - 1);
    }
    if (getSoundTimer() > 0) {
        setSoundTimer(getSoundTimer() - 1);
    }
}

// int main() {

//     return 0;
// }