#ifndef RENDERER_H
#define RENDERER_H

#include "../Model/Game.h"
#include <string>

class Renderer {
public:
    Renderer();

    void render(const Game& game);
    void renderMenu();
    void renderGameOver(const Game& game);
    void renderPaused(const Game& game);

    void clearScreen();
    void setCursorPosition(int x, int y);
    void hideCursor();
    void showCursor();

private:
    void renderBoard(const Game& game);
    void renderCurrentPiece(const Game& game);
    void renderGhostPiece(const Game& game);
    void renderSidebar(const Game& game);
    void renderNextPiece(const Tetromino& tetromino, int startX, int startY);

    char getCellChar(int value) const;
    std::string getColorCode(int value) const;
    void resetColor();

    static const int BOARD_OFFSET_X = 2;
    static const int BOARD_OFFSET_Y = 1;
};

#endif
