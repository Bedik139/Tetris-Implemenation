#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Tetromino.h"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
public:
    Game();

    void start();
    void pause();
    void resume();
    void reset();

    bool moveLeft();
    bool moveRight();
    bool moveDown();
    void hardDrop();
    void rotate();
    void rotateCounterClockwise();

    void update();
    void spawnNewTetromino();

    int getScore() const;
    int getLevel() const;
    int getLinesCleared() const;
    GameState getState() const;

    const Board& getBoard() const;
    const Tetromino& getCurrentTetromino() const;
    const Tetromino& getNextTetromino() const;
    int getCurrentX() const;
    int getCurrentY() const;
    int getGhostY() const;

    double getDropInterval() const;

private:
    Board board;
    Tetromino currentTetromino;
    Tetromino nextTetromino;

    int currentX;
    int currentY;

    int score;
    int level;
    int linesCleared;
    int totalLinesCleared;

    GameState state;

    void lockTetromino();
    void updateScore(int lines);
    void updateLevel();
    int calculateGhostY() const;

    static const int LINES_PER_LEVEL = 10;
    static const int BASE_SCORE_PER_LINE[];
};

#endif
