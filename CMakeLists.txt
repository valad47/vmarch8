cmake_minimum_required(VERSION 3.30)
project(vmarch8)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

set(RAYLIB_VERSION 4.5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

FetchContent_MakeAvailable(raylib)

add_executable(vmarch8 src/main.c)
set_property(TARGET vmarch8 PROPERTY C_STANDARD 23)
target_link_libraries(vmarch8 raylib)
