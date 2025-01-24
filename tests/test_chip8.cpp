#include <iostream>
#include "catch_amalgamated.hpp"
#include "../chip8.hpp"

TEST_CASE("Test Opcode 0x00E0", "[0x00e0]") {
    Chip8 chip8;

    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            chip8.setGraphics(y, x, 1);
        }
    }

    chip8.setOpcode(0x00E0);
    chip8.Emulate();

    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            REQUIRE(chip8.getGraphics(y, x) == 0);
        }
    }
}

TEST_CASE("Test Opcode 0x00EE", "[0x00ee]") {
    Chip8 chip8;

    chip8.setPtr(2);
    chip8.setStack(1, 0x300);

    chip8.setOpcode(0x00EE);
    chip8.Emulate();

    REQUIRE(chip8.getPtr() == 1);
    REQUIRE(chip8.getPC() == 0x300);
}

TEST_CASE("Test Opcode 0xANNN", "[0xannn]") {
    Chip8 chip8;

    chip8.setOpcode(0xA123);
    chip8.Emulate();

    REQUIRE(chip8.getI() == 0x0123);
}