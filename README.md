# CHIP-8
A fully-functional CHIP-8 interpreter written in C and SDL2. Mimics the behaviour of the COSMAC VIP with all instructions implemented as well as sound.
## Build
Tested on both Windows using CLion and MinGW, and Linux using the command line. \
Ensure vcpkg is installed and environment variables are setup as described in this [guide](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash).
### Dependencies
- GCC
- Ninja
- CMake >= 3.19
- libsdl2-dev (Vcpkg won't install all sdl dependencies by itself)
```shell
mkdir chip8
git clone https://github.com/mattkolega/chip8.git
cd chip8
mkdir build
cmake -B build -S . --preset=linux
cmake --build build
```
## Usage
Navigate to the build folder and run `./chip8`.
The interpreter requires `.ch8` files as ROM files which are chosen with a file dialog.
### Controls
Original CHIP-8 computers supported hexadecimal keypads for input. The interpreter maps those keys to the left side of a qwerty keyboard.
The keys needed to be pressed during execution depend on the ROM file.
```
+---+---+---+---+      +---+---+---+---+
| 1 | 2 | 3 | C |      | 1 | 2 | 3 | 4 |
+---+---+---+---+      +---+---+---+---+
| 4 | 5 | 6 | D |      | Q | W | E | R |
+---+---+---+---+  ->  +---+---+---+---+
| 7 | 8 | 9 | E |      | A | S | D | F |
+---+---+---+---+      +---+---+---+---+
| A | 0 | B | F |      | Z | X | C | V |
+---+---+---+---+      +---+---+---+---+
```
The `left` and `right` arrow keys can be pressed during ROM execution to cycle through colour schemes for the display window.
## Future Ideas
- Implement additional CHIP-8 specifications such as SUPER-CHIP or XO-CHIP
- Add configuration options for the interpreter using a `.ini` file
- Add debugger/disassembler to monitor stack, registers, program counter, etc
- Add unit tests for opcodes
## Acknowledgements
- [Tobias V. Langhoff](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) for his overview and guide on implementing a CHIP-8 interpreter.
- [Timendus](https://github.com/Timendus/chip8-test-suite) for their test ROM collection which greatly helped with getting all the opcodes working properly.
- [CHIP-8 Variant Opcode Table](https://chip8.gulrak.net/) for aiding with identifying quirks related to the COSMAC VIP implementation
## Licence
[MIT](LICENSE)
