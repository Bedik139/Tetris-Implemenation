#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

enum class InputAction {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    HARD_DROP,
    ROTATE_CW,
    ROTATE_CCW,
    PAUSE,
    QUIT,
    START,
    RESTART
};

class InputHandler {
public:
    InputHandler();

    InputAction getInput();
    bool isKeyPressed();

private:
    void setupConsole();
    void restoreConsole();
};

#endif
