#include "../../include/Model/Board.h"

Board::Board() {
    clear();
}

void Board::clear() {
    grid.clear();
    grid.resize(HEIGHT, std::vector<int>(WIDTH, 0));
}

bool Board::canPlace(const Tetromino& tetromino, int x, int y) const {
    const auto& shape = tetromino.getShape();

    for (int row = 0; row < Tetromino::MATRIX_SIZE; ++row) {
        for (int col = 0; col < Tetromino::MATRIX_SIZE; ++col) {
            if (shape[row][col] != 0) {
                int boardX = x + col;
                int boardY = y + row;

                // Check bounds
                if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT) {
                    return false;
                }

                // Allow placement above the board (for spawning)
                if (boardY < 0) {
                    continue;
                }

                // Check collision with existing blocks
                if (grid[boardY][boardX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Board::place(const Tetromino& tetromino, int x, int y) {
    const auto& shape = tetromino.getShape();
    int typeValue = static_cast<int>(tetromino.getType()) + 1; // +1 so 0 remains empty

    for (int row = 0; row < Tetromino::MATRIX_SIZE; ++row) {
        for (int col = 0; col < Tetromino::MATRIX_SIZE; ++col) {
            if (shape[row][col] != 0) {
                int boardX = x + col;
                int boardY = y + row;

                if (boardY >= 0 && boardY < HEIGHT && boardX >= 0 && boardX < WIDTH) {
                    grid[boardY][boardX] = typeValue;
                }
            }
        }
    }
}

int Board::clearLines() {
    int linesCleared = 0;

    for (int row = HEIGHT - 1; row >= 0; --row) {
        if (isRowFull(row)) {
            clearRow(row);
            shiftRowsDown(row);
            ++linesCleared;
            ++row; // Check this row again since rows shifted down
        }
    }

    return linesCleared;
}

int Board::getCell(int x, int y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return -1;
    }
    return grid[y][x];
}

bool Board::isRowFull(int row) const {
    if (row < 0 || row >= HEIGHT) {
        return false;
    }

    for (int col = 0; col < WIDTH; ++col) {
        if (grid[row][col] == 0) {
            return false;
        }
    }
    return true;
}

bool Board::isGameOver() const {
    // Check if any blocks are in the top two rows (spawn area)
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < WIDTH; ++col) {
            if (grid[row][col] != 0) {
                return true;
            }
        }
    }
    return false;
}

const std::vector<std::vector<int>>& Board::getGrid() const {
    return grid;
}

void Board::clearRow(int row) {
    for (int col = 0; col < WIDTH; ++col) {
        grid[row][col] = 0;
    }
}

void Board::shiftRowsDown(int fromRow) {
    for (int row = fromRow; row > 0; --row) {
        for (int col = 0; col < WIDTH; ++col) {
            grid[row][col] = grid[row - 1][col];
        }
    }
    // Clear top row
    for (int col = 0; col < WIDTH; ++col) {
        grid[0][col] = 0;
    }
}
