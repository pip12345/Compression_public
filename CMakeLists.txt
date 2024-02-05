cmake_minimum_required(VERSION 3.22) # The minimum CMake version used.
project(Compression) # Name of the current project.

add_executable(Compression) # Define a name for executable.

set(target Compression) # Create a name for target.

# Add the files to add to executable.
target_sources(${target}
        PRIVATE
        main.cpp # The main file of the project.

        ${CMAKE_SOURCE_DIR}/include/huff.h
        ${CMAKE_SOURCE_DIR}/include/lzw.h
        ${CMAKE_SOURCE_DIR}/include/file_handler.h
        ${CMAKE_SOURCE_DIR}/include/compression_handler.h

        ${CMAKE_SOURCE_DIR}/src/huff.cpp
        ${CMAKE_SOURCE_DIR}/src/lzw.cpp
        ${CMAKE_SOURCE_DIR}/src/file_handler.cpp
        ${CMAKE_SOURCE_DIR}/src/compression_handler.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include) # Instruct the compiler where to look for the given header files within this project.
include_directories(${CMAKE_SOURCE_DIR}/src) # Instruct the compiler where to look for the given src files within this project.