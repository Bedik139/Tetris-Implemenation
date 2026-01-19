cmake_minimum_required(VERSION 3.10)
project(Tetris VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/Model/Tetromino.cpp
    src/Model/Board.cpp
    src/Model/Game.cpp
    src/View/Renderer.cpp
    src/Controller/InputHandler.cpp
    src/Controller/GameController.cpp
)

# Header files
set(HEADERS
    include/Model/Tetromino.h
    include/Model/Board.h
    include/Model/Game.h
    include/View/Renderer.h
    include/Controller/InputHandler.h
    include/Controller/GameController.h
)

# Create executable
add_executable(Tetris ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(Tetris PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Windows-specific settings
if(WIN32)
    target_compile_definitions(Tetris PRIVATE _WIN32)
endif()

# Enable warnings
if(MSVC)
    target_compile_options(Tetris PRIVATE /W4)
else()
    target_compile_options(Tetris PRIVATE -Wall -Wextra -Wpedantic)
endif()
