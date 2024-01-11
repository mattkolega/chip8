cmake_minimum_required(VERSION 3.26)
project(chip-8 C)

set(CMAKE_C_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)

add_executable(chip-8 src/main.c
        src/vm.c
        src/vm.h
        src/interpreter.c
        src/interpreter.h
        src/utils.c
        src/utils.h
        src/instructions.c
        src/instructions.h
)

target_link_libraries(chip-8
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)