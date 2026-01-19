#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "../Model/Game.h"
#include "../View/Renderer.h"
#include "InputHandler.h"
#include <chrono>

class GameController {
public:
    GameController();
    ~GameController();

    void run();

private:
    Game game;
    Renderer renderer;
    InputHandler inputHandler;

    bool running;
    std::chrono::steady_clock::time_point lastDropTime;
    std::chrono::steady_clock::time_point lastRenderTime;

    void handleInput();
    void update();
    void render();

    void handleMenuInput(InputAction action);
    void handlePlayingInput(InputAction action);
    void handlePausedInput(InputAction action);
    void handleGameOverInput(InputAction action);

    bool shouldDrop();
    void resetDropTimer();

    static const int TARGET_FPS = 60;
    static const int FRAME_DURATION_MS = 1000 / TARGET_FPS;
};

#endif
