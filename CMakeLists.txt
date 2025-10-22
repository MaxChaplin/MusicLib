cmake_minimum_required(VERSION 3.14)

project(MusicLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wformat -Wall -Wextra -pedantic -Wunreachable-code -Wunused -Wunused-function)

# Add PortAudio library
include(FetchContent)

FetchContent_Declare(
  portaudio
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG master
)
FetchContent_MakeAvailable(portaudio)

# Create the main library
file(GLOB SRC "src/*.cpp")
file(GLOB INC "inc/*.hpp")
add_library(musiclib ${SRC} ${INC})

target_include_directories(
  musiclib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
  musiclib
  PUBLIC portaudio
)