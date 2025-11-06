cmake_minimum_required(VERSION 3.14)

project(DemoCustomFormat1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wformat -Wall -Wextra -pedantic -Wunreachable-code -Wunused -Wunused-function)

# Add MusicLib
set(MUSICLIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(MUSICLIB_BUILD "${MUSICLIB_ROOT}/build/debug")
else()
    set(MUSICLIB_BUILD "${MUSICLIB_ROOT}/build")
endif()

find_library(MUSICLIB_LIB
    NAMES musiclib
    PATHS ${MUSICLIB_BUILD}
)

# Create executable
file(GLOB SRC "*.cpp")
add_executable(demo ${SRC})

target_include_directories(
    demo
    PRIVATE ${MUSICLIB_ROOT}/inc
)

target_link_libraries(
    demo
    PRIVATE ${MUSICLIB_LIB}
    portaudio
)