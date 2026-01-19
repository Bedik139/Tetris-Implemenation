#include "../../include/Controller/GameController.h"
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

GameController::GameController()
    : running(false)
    , lastDropTime(std::chrono::steady_clock::now())
    , lastRenderTime(std::chrono::steady_clock::now()) {
}

GameController::~GameController() {
    renderer.showCursor();
    renderer.clearScreen();
}

void GameController::run() {
    running = true;
    renderer.clearScreen();
    renderer.renderMenu();

    while (running) {
        handleInput();

        if (game.getState() == GameState::PLAYING) {
            update();
        }

        render();

        // Frame rate limiting
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
}

void GameController::handleInput() {
    InputAction action = inputHandler.getInput();

    if (action == InputAction::NONE) {
        return;
    }

    switch (game.getState()) {
        case GameState::MENU:
            handleMenuInput(action);
            break;
        case GameState::PLAYING:
            handlePlayingInput(action);
            break;
        case GameState::PAUSED:
            handlePausedInput(action);
            break;
        case GameState::GAME_OVER:
            handleGameOverInput(action);
            break;
    }
}

void GameController::handleMenuInput(InputAction action) {
    switch (action) {
        case InputAction::START:
            game.start();
            renderer.clearScreen();
            resetDropTimer();
            break;
        case InputAction::QUIT:
            running = false;
            break;
        default:
            break;
    }
}

void GameController::handlePlayingInput(InputAction action) {
    switch (action) {
        case InputAction::MOVE_LEFT:
            game.moveLeft();
            break;
        case InputAction::MOVE_RIGHT:
            game.moveRight();
            break;
        case InputAction::MOVE_DOWN:
            if (game.moveDown()) {
                resetDropTimer();
            }
            break;
        case InputAction::HARD_DROP:
            game.hardDrop();
            resetDropTimer();
            break;
        case InputAction::ROTATE_CW:
            game.rotate();
            break;
        case InputAction::ROTATE_CCW:
            game.rotateCounterClockwise();
            break;
        case InputAction::PAUSE:
            game.pause();
            break;
        case InputAction::QUIT:
            running = false;
            break;
        default:
            break;
    }
}

void GameController::handlePausedInput(InputAction action) {
    switch (action) {
        case InputAction::PAUSE:
            game.resume();
            renderer.clearScreen();
            break;
        case InputAction::QUIT:
            running = false;
            break;
        default:
            break;
    }
}

void GameController::handleGameOverInput(InputAction action) {
    switch (action) {
        case InputAction::RESTART:
            game.start();
            renderer.clearScreen();
            resetDropTimer();
            break;
        case InputAction::QUIT:
            running = false;
            break;
        default:
            break;
    }
}

void GameController::update() {
    if (shouldDrop()) {
        game.update();
        resetDropTimer();
    }
}

void GameController::render() {
    switch (game.getState()) {
        case GameState::MENU:
            // Menu is rendered once when entering, no need to re-render
            break;
        case GameState::PLAYING:
            renderer.render(game);
            break;
        case GameState::PAUSED:
            renderer.render(game);
            renderer.renderPaused(game);
            break;
        case GameState::GAME_OVER:
            renderer.renderGameOver(game);
            break;
    }
}

bool GameController::shouldDrop() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDropTime);
    return elapsed.count() >= game.getDropInterval();
}

void GameController::resetDropTimer() {
    lastDropTime = std::chrono::steady_clock::now();
}
