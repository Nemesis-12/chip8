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
    pc = 0x200;                 // Games are loaded at this location
    I = 0;                      // Reset address register
    memset(V, 0, sizeof(V));    // Set registers to zero
    srand(time(NULL));

    // Load font set onto memory
    for (int i = 0; i < 80; ++i) {
        memory[i] = fontSet[i];
    }
}

void Chip8::Load(char const* filename) {
    // Read game onto memory as a binary file and move pointer to the end
    filename = " ";
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
            memory[pc + i] = buffer[i];
        }

        delete[] buffer;
    }
}

void Chip8::Emulate() {
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    pc += 2;
    uint8_t Vx = (opcode & 0x00FF) >> 8;
    uint8_t Vy = (opcode & 0x00FF) >> 4;

    uint16_t sum = 0;
    uint8_t randByte = rand() % 256;

    // Decode opcode
    // Check first 4 bits and decide what to do
    switch(opcode & 0xF000) {
        case 0x0000:
            // 0x00E0 - Clear screen
            if (opcode == 0x00E0) {
                memset(graphics, 0, sizeof(graphics));
            }

            // 0x00EE - Return from subroutine
            else if (opcode == 0x00EE) {
                --ptr;
                pc = stack[ptr];
            }
            break;

        // 0xANNN - Jump to address
        case 0xA000:
            I = opcode & 0x0FFF;
            break;

        // 0x2NNN - Call subroutine at address
        case 0x2000:
            stack[ptr] = pc;
            ++ptr;
            pc = opcode & 0x0FFF;
            break;

        // 0x3XKK - Skip next instruction if Vx = KK
        case 0x3000:
            if (Vx == (opcode & 0x00FF)) {
                pc += 2;
            }
            break;

        // 0x4XKK - Skip next instruction if Vx != KK
        case 0x4000:
            if (Vx != (opcode & 0x00FF)) {
                pc += 2;
            }
            break;

        // 0x5XY0 - Skip next instruction if Vx = Vy
        case 0x5000:
            if (Vx == Vy) {
                pc += 2;
            }
            break;

        // 0x6XKK - Set Vx = KK
        case 0x6000:
            Vx = (opcode & 0x00FF);
            break;

        // 0x7XKK - Set Vx = Vx + KK
        case 0x7000:
            Vx += (opcode & 0x00FF);
            break;

        // 0x8000 instruction set
        case 0x8000:
            // Check last bits of code to determine the instruction
            switch(opcode & 0x000F) {

                // 0x8XY0 - Set Vx = Vy
                case 0x0000:
                    Vx = Vy;
                    break;

                // 0x8XY1 - Set Vx = Vx OR Vy
                case 0x0001:
                    Vx |= Vy;
                    break;

                // 0x8XY2 - Set Vx = Vx AND Vy
                case 0x0002:
                    Vx &= Vy;
                    break;
                
                // 0x8XY3 - Set Vx = Vx XOR Vy
                case 0x0003:
                    Vx ^= Vy;
                    break;

                // 0x8XY4 - Set Vx = Vx + Vy
                case 0x0004:
                    sum = Vx + Vy;
                    V[0xF] = sum > 255 ? 1 : 0;
                    Vx = sum & 0xFF;

                    break;
                
                // 0x8XY5 - Set Vx = Vx - Vy
                case 0x0005:
                    V[0xF] = Vx > Vy ? 1 : 0;
                    Vx -= Vy;

                    break;

                // 0x8XY6 - Set Vx = Vx >> 1
                case 0x0006:
                    // Save LSB
                    V[0xF] = Vx & 0x1;
                    Vx >>= 1;

                    break;
                
                // 0x8XY7 - Set Vx = Vy - Vx
                case 0x0007:
                    V[0xF] = Vx < Vy ? 1 : 0;
                    Vx = Vy - Vx;

                    break;

                // 0x8XYE - Set Vx = Vx << 1
                case 0x000E:
                    // Save MSB
                    V[0xF] = Vx >> 0x7;
                    Vx <<= 1;

                    break;
            }
            break;

        // 0x9XY0 - Skip next instruction if Vx != Vy
        case 0x9000:
            if (Vx != Vy) {
                pc += 2;
            }
            break;

        // 0xBNNN - Jump to location NNN + V0
        case 0xB000:
            pc = V[0] + (opcode & 0x0FFF);
            break;

        // 0xCXY0 - Set Vx = random byte AND kk
        case 0xC000:
            Vx = randByte & (opcode & 0x00FF);
            break;

        // 0xDXYN - Display n-byte sprite at location (Vx, Vy) and set
        // VF = collision
        case 0xD000:
            uint8_t height = opcode & 0x000F;
            uint8_t sprite;
            uint8_t x = Vx % 64;
            uint8_t y = Vy % 32;
            // Reset before drawing
            V[0xF] = 0;

            for (auto i = 0; i < height; i++) {
                sprite = memory[I + i];

                for (auto j = 0; j < 8; j++) {
                    if ((sprite & (0x80 >> j)) != 0) {
                        if (graphics[(Vx + j + ((Vy + i) * 64)) % 2048] == 1) {
                            V[0xF] = 1;
                        }

                        graphics[(Vx + j + ((Vy + i) * 64)) % 2048] ^= 1;
                    }
                }
            }
            pc += 2;

            break;

        // EX00
        case 0xE000:
            switch(opcode & 0x000F) {

                case 0x000E:
                    // EX9E - Skip next instruction if Vx is pressed
                    uint8_t key = V[Vx];

                    if (!keypad[key]) {
                        pc += 2;
                    }

                    break;
            }

            break;
    }

    // Decrement timers when set
    if (delay_timer > 1) { --delay_timer; }
    if (sound_timer > 1) { --sound_timer; }
}

int main() {

    return 0;
}