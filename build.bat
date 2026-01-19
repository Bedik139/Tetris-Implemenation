@echo off
echo Building Tetris...

if not exist "build" mkdir build

g++ -std=c++17 -Wall -Wextra ^
    src/main.cpp ^
    src/Model/Tetromino.cpp ^
    src/Model/Board.cpp ^
    src/Model/Game.cpp ^
    src/View/Renderer.cpp ^
    src/Controller/InputHandler.cpp ^
    src/Controller/GameController.cpp ^
    -I include ^
    -o build/Tetris.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run with: build\Tetris.exe
) else (
    echo Build failed!
)
