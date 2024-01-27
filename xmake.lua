add_rules("mode.debug", "mode.release")

add_requires("libsdl", "miniaudio", "tinyfiledialogs")

set_languages("c17")

target("chip8")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("libsdl", "miniaudio", "tinyfiledialogs")