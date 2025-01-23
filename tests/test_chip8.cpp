#include <iostream>
#include "catch_amalgamated.hpp"
#include "../chip8.hpp"

TEST_CASE("Test Chip8 Initialization", "[init_chip8]") {
    Chip8 chip8;

    std::cout << "Starting test..." << std::endl;

    REQUIRE(chip8.getPC() == 0x200);

    REQUIRE(chip8.getI() == 0);

    for (int i = 0; i < 16; ++i) {
        REQUIRE(chip8.getV(i) == 0);
    }
}

TEST_CASE("Test Opcode 0x00E0", "[0x00e0]") {
    Chip8 chip8;

    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            chip8.setGraphics(y, x, 1);
        }
    }

    chip8.setOpcode(0x00E0);
    if (chip8.getOpcode() == 0x00E0) {
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                chip8.setGraphics(y, x, 0);
            }
        }
    }

    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            REQUIRE(chip8.getGraphics(y, x) == 0);
        }
    }
}

TEST_CASE("Test Opcode 0x00EE", "[0x00ee]") {
    Chip8 chip8;

    chip8.setOpcode(0x00EE);
    chip8.setPtr(2);
    chip8.setStack(1, 0x300);

    if (chip8.getOpcode() == 0x00EE) {
        chip8.setPtr(chip8.getPtr() - 1);
        chip8.setPC(chip8.getStack(chip8.getPtr()));
    }

    REQUIRE(chip8.getPtr() == 1);
    REQUIRE(chip8.getPC() == 0x300);

    chip8.setPtr(5);
    chip8.setStack(4, 0x250);

    if (chip8.getOpcode() == 0x00EE) {
        chip8.setPtr(chip8.getPtr() - 1);
        chip8.setPC(chip8.getStack(chip8.getPtr()));
    }

    REQUIRE(chip8.getPtr() == 4);
    REQUIRE(chip8.getPC() == 0x250);

}