cmake_minimum_required(VERSION 3.19)
project(chip8 C)

set(CMAKE_C_STANDARD 17)

find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")

find_package(SDL2 CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.c
        src/vm.c
        src/vm.h
        src/application.c
        src/application.h
        src/utils.c
        src/utils.h
        src/instructions.c
        src/instructions.h
        src/audio.c
        src/audio.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${MINIAUDIO_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        tinyfiledialogs::tinyfiledialogs
)