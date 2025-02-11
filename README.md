# CHIP-8 Emulator

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Development Progress](#development-progress)
- [Prerequisites](#prerequisites)
- [Build and Run](#build-and-run)
- [Testing](#testing)
- [Acknowledgements](#acknowledgements)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The CHIP-8 emulator is a project that replicates the behavior of the CHIP-8 virtual machine, an interpreted language developed by Joseph Weisbecker in the 1970s. The project aims to provide a working environment to run CHIP-8 games and programs.

## Features

- CHIP-8 Instruction Set: Implements core opcodes categorized by functionality.
- Cross-Platform Testing: Utilizes Catch2 for unit testing and a GitHub Actions CI/CD pipeline for Linux, macOS, and Windows builds.
- SDL/OpenGL Integration (Planned): Will include a graphical user interface for rendering and user interactions.

## Development Progress

### Implemented

🟩 Display Opcodes - 00E0, DXYN  
🟩 Control Flow Opcodes - 1NNN, 2NNN, 00EE, BNNN  
🟩 Arithmetic and Logic Opcodes - 8XY0 to 8XYE   
🟩 Conditional Statement Opcodes - 3XNN, 4XNN, 5XY0, 9XY0  
🟨 Memory Management Opcodes - ANNN  
🟨 Register Operations - 6XNN, 7XNN  

### Planned

🟨 Register Operations - FX1E  
🟨 Memory Management Opcodes - FX29, FX33, FX55, FX65  
🟥 Input Handling - EX9E, EXA1, FX0A  
🟥 Timers - FX07, FX15  
🟥 SDL/OpenGL Integration  

## Prerequisites

- Compiler: A C++17-compatible compiler:
  - Clang++
  - G++
- Libraries:
  - SDL/OpenGL for rendering (to be added in the future).
  - Catch2 for testing (included in the repo).

## Build and Run

To build and run the emulator:

1. Clone the repository:

```console
git clone https://github.com/Nemesis-12/chip8.git
cd chip8-emulator
```

2. Compile the project using Clang++ or G++:

```console
clang++ -std=c++17 chip8.cpp -o chip8
```
```console
g++ -std=c++17 chip8.cpp -o chip8
```

3. Execute the emulator:

```console
./chip8
```

## Testing

This project uses Catch2 as the testing framework. Additionally, a GitHub Actions workflow is implemented to automate CI/CD processes, including building the project and running tests, ensuring compatibility across Linux, MacOS, and Windows platforms.

### Running Tests Locally

1. Compile the tests using Clang++ or G++:

```console
clang++ -std=c++17 -DTESTING -Ichip8/chip8 tests/catch_amalgamated.cpp tests/test_chip8.cpp chip8.cpp -o tests/chip8_tests.exe
```

```console
g++ -std=c++17 -DTESTING -Ichip8/chip8 tests/catch_amalgamated.cpp tests/test_chip8.cpp chip8.cpp -o tests/chip8_tests.exe
```

2. Execute the test suite:

```console
./tests/chip8_tests.exe
```

## Acknowledgements

- [Catch2](https://github.com/catchorg/Catch2) - This project uses the C++ test framework Catch2 for unit tests. Thanks to the Catch2 authors and contributors for their efforts in maintaining the library.

## Contributing
This project is open to contributions. To contribute:
- Fork the repository.
- Make your changes.
- Submit a pull request with a description of your updates.

Feel free to open an issue for suggestions or bugs.

## License
This project is licensed under the [MIT License](LICENSE).