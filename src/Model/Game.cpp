#include "../../include/Model/Game.h"

// Scoring based on original Nintendo scoring system
const int Game::BASE_SCORE_PER_LINE[] = {0, 40, 100, 300, 1200};

Game::Game()
    : currentX(0)
    , currentY(0)
    , score(0)
    , level(1)
    , linesCleared(0)
    , totalLinesCleared(0)
    , state(GameState::MENU) {
}

void Game::start() {
    reset();
    state = GameState::PLAYING;
}

void Game::pause() {
    if (state == GameState::PLAYING) {
        state = GameState::PAUSED;
    }
}

void Game::resume() {
    if (state == GameState::PAUSED) {
        state = GameState::PLAYING;
    }
}

void Game::reset() {
    board.clear();
    score = 0;
    level = 1;
    linesCleared = 0;
    totalLinesCleared = 0;

    currentTetromino = Tetromino::createRandom();
    nextTetromino = Tetromino::createRandom();

    // Spawn position: centered at top
    currentX = (Board::WIDTH - Tetromino::MATRIX_SIZE) / 2;
    currentY = 0;
}

bool Game::moveLeft() {
    if (state != GameState::PLAYING) return false;

    if (board.canPlace(currentTetromino, currentX - 1, currentY)) {
        --currentX;
        return true;
    }
    return false;
}

bool Game::moveRight() {
    if (state != GameState::PLAYING) return false;

    if (board.canPlace(currentTetromino, currentX + 1, currentY)) {
        ++currentX;
        return true;
    }
    return false;
}

bool Game::moveDown() {
    if (state != GameState::PLAYING) return false;

    if (board.canPlace(currentTetromino, currentX, currentY + 1)) {
        ++currentY;
        return true;
    }
    return false;
}

void Game::hardDrop() {
    if (state != GameState::PLAYING) return;

    while (board.canPlace(currentTetromino, currentX, currentY + 1)) {
        ++currentY;
        score += 2; // Bonus points for hard drop
    }
    lockTetromino();
}

void Game::rotate() {
    if (state != GameState::PLAYING) return;

    Tetromino rotated = currentTetromino;
    rotated.rotate();

    // Try normal rotation
    if (board.canPlace(rotated, currentX, currentY)) {
        currentTetromino = rotated;
        return;
    }

    // Wall kick: try moving left or right
    if (board.canPlace(rotated, currentX - 1, currentY)) {
        currentTetromino = rotated;
        --currentX;
        return;
    }

    if (board.canPlace(rotated, currentX + 1, currentY)) {
        currentTetromino = rotated;
        ++currentX;
        return;
    }

    // Try moving 2 spaces (for I-piece)
    if (board.canPlace(rotated, currentX - 2, currentY)) {
        currentTetromino = rotated;
        currentX -= 2;
        return;
    }

    if (board.canPlace(rotated, currentX + 2, currentY)) {
        currentTetromino = rotated;
        currentX += 2;
        return;
    }
}

void Game::rotateCounterClockwise() {
    if (state != GameState::PLAYING) return;

    Tetromino rotated = currentTetromino;
    rotated.rotateCounterClockwise();

    if (board.canPlace(rotated, currentX, currentY)) {
        currentTetromino = rotated;
        return;
    }

    // Wall kicks
    if (board.canPlace(rotated, currentX - 1, currentY)) {
        currentTetromino = rotated;
        --currentX;
        return;
    }

    if (board.canPlace(rotated, currentX + 1, currentY)) {
        currentTetromino = rotated;
        ++currentX;
        return;
    }
}

void Game::update() {
    if (state != GameState::PLAYING) return;

    if (!moveDown()) {
        lockTetromino();
    }
}

void Game::spawnNewTetromino() {
    currentTetromino = nextTetromino;
    nextTetromino = Tetromino::createRandom();

    currentX = (Board::WIDTH - Tetromino::MATRIX_SIZE) / 2;
    currentY = 0;

    // Check if new piece can be placed
    if (!board.canPlace(currentTetromino, currentX, currentY)) {
        state = GameState::GAME_OVER;
    }
}

int Game::getScore() const {
    return score;
}

int Game::getLevel() const {
    return level;
}

int Game::getLinesCleared() const {
    return totalLinesCleared;
}

GameState Game::getState() const {
    return state;
}

const Board& Game::getBoard() const {
    return board;
}

const Tetromino& Game::getCurrentTetromino() const {
    return currentTetromino;
}

const Tetromino& Game::getNextTetromino() const {
    return nextTetromino;
}

int Game::getCurrentX() const {
    return currentX;
}

int Game::getCurrentY() const {
    return currentY;
}

int Game::getGhostY() const {
    return calculateGhostY();
}

double Game::getDropInterval() const {
    // Speed increases with level (milliseconds between drops)
    // Level 1: 1000ms, Level 10: ~100ms, Level 20: ~50ms
    double baseInterval = 1000.0;
    double speedFactor = 1.0 - (level - 1) * 0.1;
    if (speedFactor < 0.05) speedFactor = 0.05;
    return baseInterval * speedFactor;
}

void Game::lockTetromino() {
    board.place(currentTetromino, currentX, currentY);

    int lines = board.clearLines();
    if (lines > 0) {
        updateScore(lines);
        totalLinesCleared += lines;
        updateLevel();
    }

    if (board.isGameOver()) {
        state = GameState::GAME_OVER;
    } else {
        spawnNewTetromino();
    }
}

void Game::updateScore(int lines) {
    if (lines > 0 && lines <= 4) {
        score += BASE_SCORE_PER_LINE[lines] * level;
    }
}

void Game::updateLevel() {
    int newLevel = (totalLinesCleared / LINES_PER_LEVEL) + 1;
    if (newLevel > level) {
        level = newLevel;
    }
}

int Game::calculateGhostY() const {
    int ghostY = currentY;
    while (board.canPlace(currentTetromino, currentX, ghostY + 1)) {
        ++ghostY;
    }
    return ghostY;
}
