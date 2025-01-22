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