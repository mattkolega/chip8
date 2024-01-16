# CHIP-8
A fully-functional CHIP-8 interpreter written in C and SDL2. Mimics the behaviour of the COSMAC VIP with all instructions implemented as well as sound.
## Build
Tested on both Windows using CLion and MinGW, and Linux using the command line. \
Ensure vcpkg is installed and environment variables are setup as described in this [guide](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash).
### Dependencies:
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
Navigate to the build folder and run ```./chip8```.
The program will prompt the user to choose the ROM through a file dialog and the interpreter will then start running.
## Acknowledgements
- [Tobias V. Langhoff](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) for his overview and guide on implementing a CHIP-8 interpreter.
- [Timendus](https://github.com/Timendus/chip8-test-suite) for their test ROM collection which greatly helped with getting all the opcodes working properly.
- [CHIP-8 Variant Opcode Table](https://chip8.gulrak.net/) for aiding with identifying quirks related to the COSMAC VIP implementation
## Licence
[MIT](LICENSE)