#include <iostream>
#include "catch_amalgamated.hpp"
#include "../chip8.hpp"

// Test for Opcode 0x00E0
TEST_CASE("Test Opcode 0x00E0", "[0x00e0]") {
    Chip8 chip8;

    // Set all pixels to 1 for testing
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            chip8.setGraphics(y, x, 1);
        }
    }

    // Set test value for opcode and emulate it
    chip8.setOpcode(0x00E0);
    chip8.Emulate();

    // Check if test passed
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            REQUIRE(chip8.getGraphics(y, x) == 0);
        }
    }
}

// Test for Opcode 0x00EE
TEST_CASE("Test Opcode 0x00EE", "[0x00ee]") {
    Chip8 chip8;

    // Set test values for pointer and stack
    chip8.setPtr(2);
    chip8.setStack(1, 0x300);

    // Set test value for opcode and emulate it
    chip8.setOpcode(0x00EE);
    chip8.Emulate();

    // Check if test passed
    REQUIRE(chip8.getPtr() == 1);
    REQUIRE(chip8.getPC() == 0x300);
}

// Test for Opcode 0xANNN
TEST_CASE("Test Opcode 0xANNN", "[0xannn]") {
    Chip8 chip8;

    // Set test value for opcode and emulate it
    chip8.setOpcode(0xA123);
    chip8.Emulate();

    // Check if test passed
    REQUIRE(chip8.getI() == 0x0123);
}

// Test for Opcode 0x1NNN
TEST_CASE("Test Opcode 0x1NNN", "[0x1nnn]") {
    Chip8 chip8;

    // Set test value for opcode and emulate it
    chip8.setOpcode(0x1123);
    chip8.Emulate();

    // Check if test passed
    REQUIRE(chip8.getPC() == 0x0123);
}

// Test for Opcode 0x2NNN
TEST_CASE("Test Opcode 0x2NNN", "[0x2nnn]") {
    Chip8 chip8;

    // Set test value for pointer
    chip8.setPtr(0);

    // Set test value for opcode and emulate it
    chip8.setOpcode(0x2123);
    chip8.Emulate();

    // Check if test passed
    REQUIRE(chip8.getPtr() == 1);
    REQUIRE(chip8.getPC() == 0x0123);
}

// Test for Opcode 0xBNNN
TEST_CASE("Test Opcode 0xBNNN", "[0xbnnn]") {
    Chip8 chip8;

    // Set test value for register
    chip8.setV(0, 0x20);

    // Set test value for opcode and emulate it
    chip8.setOpcode(0xB123);
    chip8.Emulate();

    // Check if test passed
    REQUIRE(chip8.getPC() == chip8.getV(0) + 0x0123);
}

// Test for Opcode 0x3XNN
TEST_CASE("Test Opcode 0x3XNN", "[0x3xnn]") {
    Chip8 chip8;

    // Set test value for register
    chip8.setV(1, 0x20);

    // Set test value for opcode and emulate it
    chip8.setOpcode(0x3120);
    chip8.Emulate();

    // Check if test passed
    REQUIRE(chip8.getPC() == 0x204);
}

// Test for Opcode 0x4XNN
TEST_CASE("Test Opcode 0x4XNN", "[0x4xnn]") {
    Chip8 chip8;

    // Set test value for register
    chip8.setV(1, 0x20);

    // Set test value for opcode and emulate it
    chip8.setOpcode(0x4121); 
    chip8.Emulate();

    // Check if test passed
    REQUIRE(chip8.getPC() == 0x204);
}