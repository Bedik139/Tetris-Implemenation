#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Tetromino.h"

class Board {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 20;

    Board();

    void clear();
    bool canPlace(const Tetromino& tetromino, int x, int y) const;
    void place(const Tetromino& tetromino, int x, int y);
    int clearLines();

    int getCell(int x, int y) const;
    bool isRowFull(int row) const;
    bool isGameOver() const;

    const std::vector<std::vector<int>>& getGrid() const;

private:
    std::vector<std::vector<int>> grid;

    void clearRow(int row);
    void shiftRowsDown(int fromRow);
};

#endif
