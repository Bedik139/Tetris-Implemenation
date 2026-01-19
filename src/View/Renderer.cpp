#include "../../include/View/Renderer.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

Renderer::Renderer() {
    hideCursor();
}

void Renderer::clearScreen() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    std::cout << "\033[2J\033[H";
#endif
}

void Renderer::setCursorPosition(int x, int y) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(hConsole, pos);
#else
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
#endif
}

void Renderer::hideCursor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    std::cout << "\033[?25l";
#endif
}

void Renderer::showCursor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    std::cout << "\033[?25h";
#endif
}

void Renderer::render(const Game& game) {
    setCursorPosition(0, 0);
    renderBoard(game);
    renderGhostPiece(game);
    renderCurrentPiece(game);
    renderSidebar(game);
}

void Renderer::renderMenu() {
    clearScreen();
    setCursorPosition(0, 0);

    std::cout << R"(
    ╔════════════════════════════════════╗
    ║                                    ║
    ║   ████████╗███████╗████████╗██████╗ ║
    ║      ██╔══╝██╔════╝   ██╔══╝██╔══██╗║
    ║      ██║   █████╗     ██║   ██████╔╝║
    ║      ██║   ██╔══╝     ██║   ██╔══██╗║
    ║      ██║   ███████╗   ██║   ██║  ██║║
    ║      ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝║
    ║                                    ║
    ║         TETRIS CLASSIC             ║
    ║                                    ║
    ╠════════════════════════════════════╣
    ║                                    ║
    ║     Press ENTER to Start Game      ║
    ║     Press Q to Quit                ║
    ║                                    ║
    ╠════════════════════════════════════╣
    ║         CONTROLS                   ║
    ║                                    ║
    ║     LEFT/RIGHT - Move piece        ║
    ║     DOWN       - Soft drop         ║
    ║     SPACE      - Hard drop         ║
    ║     UP/Z       - Rotate            ║
    ║     X          - Rotate CCW        ║
    ║     P          - Pause             ║
    ║     Q          - Quit              ║
    ║                                    ║
    ╚════════════════════════════════════╝
)";
}

void Renderer::renderGameOver(const Game& game) {
    clearScreen();
    setCursorPosition(0, 0);

    std::cout << R"(
    ╔════════════════════════════════════╗
    ║                                    ║
    ║         ██████╗  █████╗ ███╗   ███╗███████╗║
    ║        ██╔════╝ ██╔══██╗████╗ ████║██╔════╝║
    ║        ██║  ███╗███████║██╔████╔██║█████╗  ║
    ║        ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ║
    ║        ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗║
    ║         ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝║
    ║                                    ║
    ║          ██████╗ ██╗   ██╗███████╗██████╗  ║
    ║         ██╔═══██╗██║   ██║██╔════╝██╔══██╗ ║
    ║         ██║   ██║██║   ██║█████╗  ██████╔╝ ║
    ║         ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗ ║
    ║         ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║ ║
    ║          ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝ ║
    ║                                    ║
    ╚════════════════════════════════════╝
)";

    std::cout << "\n";
    std::cout << "         Final Score: " << game.getScore() << "\n";
    std::cout << "         Level:       " << game.getLevel() << "\n";
    std::cout << "         Lines:       " << game.getLinesCleared() << "\n";
    std::cout << "\n";
    std::cout << "         Press R to Restart\n";
    std::cout << "         Press Q to Quit\n";
}

void Renderer::renderPaused(const Game& game) {
    (void)game; // Parameter reserved for future use (e.g., showing score during pause)
    int centerX = BOARD_OFFSET_X + Board::WIDTH;
    int centerY = 10;

    setCursorPosition(centerX - 4, centerY);
    std::cout << "╔══════════╗";
    setCursorPosition(centerX - 4, centerY + 1);
    std::cout << "║  PAUSED  ║";
    setCursorPosition(centerX - 4, centerY + 2);
    std::cout << "╚══════════╝";
    setCursorPosition(centerX - 6, centerY + 4);
    std::cout << "Press P to Resume";
}

void Renderer::renderBoard(const Game& game) {
    const auto& grid = game.getBoard().getGrid();

    // Top border
    setCursorPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y);
    std::cout << "╔";
    for (int i = 0; i < Board::WIDTH * 2; ++i) std::cout << "═";
    std::cout << "╗";

    // Board content
    for (int y = 0; y < Board::HEIGHT; ++y) {
        setCursorPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y + y + 1);
        std::cout << "║";

        for (int x = 0; x < Board::WIDTH; ++x) {
            int cell = grid[y][x];
            if (cell != 0) {
                std::cout << getColorCode(cell) << "[]" << "\033[0m";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << "║";
    }

    // Bottom border
    setCursorPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y + Board::HEIGHT + 1);
    std::cout << "╚";
    for (int i = 0; i < Board::WIDTH * 2; ++i) std::cout << "═";
    std::cout << "╝";
}

void Renderer::renderCurrentPiece(const Game& game) {
    const auto& shape = game.getCurrentTetromino().getShape();
    int pieceX = game.getCurrentX();
    int pieceY = game.getCurrentY();
    int colorValue = static_cast<int>(game.getCurrentTetromino().getType()) + 1;

    for (int y = 0; y < Tetromino::MATRIX_SIZE; ++y) {
        for (int x = 0; x < Tetromino::MATRIX_SIZE; ++x) {
            if (shape[y][x] != 0) {
                int screenX = BOARD_OFFSET_X + 1 + (pieceX + x) * 2;
                int screenY = BOARD_OFFSET_Y + 1 + pieceY + y;

                if (pieceY + y >= 0 && pieceY + y < Board::HEIGHT) {
                    setCursorPosition(screenX, screenY);
                    std::cout << getColorCode(colorValue) << "[]" << "\033[0m";
                }
            }
        }
    }
}

void Renderer::renderGhostPiece(const Game& game) {
    const auto& shape = game.getCurrentTetromino().getShape();
    int pieceX = game.getCurrentX();
    int ghostY = game.getGhostY();

    // Don't render ghost if it's at the same position as current piece
    if (ghostY == game.getCurrentY()) return;

    for (int y = 0; y < Tetromino::MATRIX_SIZE; ++y) {
        for (int x = 0; x < Tetromino::MATRIX_SIZE; ++x) {
            if (shape[y][x] != 0) {
                int screenX = BOARD_OFFSET_X + 1 + (pieceX + x) * 2;
                int screenY = BOARD_OFFSET_Y + 1 + ghostY + y;

                if (ghostY + y >= 0 && ghostY + y < Board::HEIGHT) {
                    setCursorPosition(screenX, screenY);
                    std::cout << "\033[90m" << ".." << "\033[0m"; // Dark gray ghost
                }
            }
        }
    }
}

void Renderer::renderSidebar(const Game& game) {
    int sidebarX = BOARD_OFFSET_X + Board::WIDTH * 2 + 5;
    int y = BOARD_OFFSET_Y + 1;

    // Next piece
    setCursorPosition(sidebarX, y++);
    std::cout << "╔═══════════╗";
    setCursorPosition(sidebarX, y++);
    std::cout << "║   NEXT    ║";
    setCursorPosition(sidebarX, y++);
    std::cout << "╠═══════════╣";

    renderNextPiece(game.getNextTetromino(), sidebarX + 2, y);
    y += 4;

    setCursorPosition(sidebarX, y++);
    std::cout << "╚═══════════╝";
    y++;

    // Score
    setCursorPosition(sidebarX, y++);
    std::cout << "╔═══════════╗";
    setCursorPosition(sidebarX, y++);
    std::cout << "║   SCORE   ║";
    setCursorPosition(sidebarX, y++);
    std::cout << "╠═══════════╣";
    setCursorPosition(sidebarX, y++);
    std::cout << "║ " << std::setw(9) << game.getScore() << " ║";
    setCursorPosition(sidebarX, y++);
    std::cout << "╚═══════════╝";
    y++;

    // Level
    setCursorPosition(sidebarX, y++);
    std::cout << "╔═══════════╗";
    setCursorPosition(sidebarX, y++);
    std::cout << "║   LEVEL   ║";
    setCursorPosition(sidebarX, y++);
    std::cout << "╠═══════════╣";
    setCursorPosition(sidebarX, y++);
    std::cout << "║     " << std::setw(2) << game.getLevel() << "    ║";
    setCursorPosition(sidebarX, y++);
    std::cout << "╚═══════════╝";
    y++;

    // Lines
    setCursorPosition(sidebarX, y++);
    std::cout << "╔═══════════╗";
    setCursorPosition(sidebarX, y++);
    std::cout << "║   LINES   ║";
    setCursorPosition(sidebarX, y++);
    std::cout << "╠═══════════╣";
    setCursorPosition(sidebarX, y++);
    std::cout << "║ " << std::setw(9) << game.getLinesCleared() << " ║";
    setCursorPosition(sidebarX, y++);
    std::cout << "╚═══════════╝";
}

void Renderer::renderNextPiece(const Tetromino& tetromino, int startX, int startY) {
    const auto& shape = tetromino.getShape();
    int colorValue = static_cast<int>(tetromino.getType()) + 1;

    for (int y = 0; y < Tetromino::MATRIX_SIZE; ++y) {
        setCursorPosition(startX - 1, startY + y);
        std::cout << "║";
        for (int x = 0; x < Tetromino::MATRIX_SIZE; ++x) {
            if (shape[y][x] != 0) {
                std::cout << getColorCode(colorValue) << "[]" << "\033[0m";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << " ║";
    }
}

std::string Renderer::getColorCode(int value) const {
    // ANSI color codes for different tetromino types
    switch (value) {
        case 1: return "\033[96m";  // I - Cyan
        case 2: return "\033[93m";  // O - Yellow
        case 3: return "\033[95m";  // T - Magenta
        case 4: return "\033[92m";  // S - Green
        case 5: return "\033[91m";  // Z - Red
        case 6: return "\033[94m";  // J - Blue
        case 7: return "\033[33m";  // L - Orange (dark yellow)
        default: return "\033[97m"; // White
    }
}

char Renderer::getCellChar(int value) const {
    if (value == 0) return ' ';
    return '#';
}

void Renderer::resetColor() {
    std::cout << "\033[0m";
}
