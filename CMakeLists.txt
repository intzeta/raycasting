cmake_minimum_required(VERSION 3.10)
project(raycasting VERSION 1.0)

set(CMAKE_C_COMPILER "clang")

find_package(SDL2 REQUIRED)

add_executable(main src/main.c src/render.c src/camera.c src/window.c)

target_link_libraries(main SDL2 m)
target_compile_options(main PRIVATE
    -Wall
    -std=c2x
    -O2
    -g
    -Wextra
    -Werror
    -Wpedantic
    -Wfloat-equal
    -Wstrict-aliasing
    -Wswitch-default
    -Wformat=2
    -Wno-newline-eof
    -Wno-unused-parameter
    -Wno-strict-prototypes
    -Wno-fixed-enum-extension
    -Wno-int-to-void-pointer-cast
    -Wno-gnu-statement-expression
    -Wno-gnu-compound-literal-initializer
    -Wno-gnu-zero-variadic-macro-arguments
    -Wno-gnu-empty-struct
    -Wno-gnu-auto-type
    -Wno-gnu-empty-initializer
    -Wno-gnu-pointer-arith
    -Wno-c99-extensions
    -Wno-c11-extensions
)
