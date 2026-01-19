#include "../../include/Controller/InputHandler.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

InputHandler::InputHandler() {
    setupConsole();
}

void InputHandler::setupConsole() {
#ifdef _WIN32
    // Enable virtual terminal processing for ANSI colors
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void InputHandler::restoreConsole() {
    // Nothing to restore on Windows
}

bool InputHandler::isKeyPressed() {
#ifdef _WIN32
    return _kbhit() != 0;
#else
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }
    return false;
#endif
}

InputAction InputHandler::getInput() {
    if (!isKeyPressed()) {
        return InputAction::NONE;
    }

#ifdef _WIN32
    int ch = _getch();

    // Handle special keys (arrows, function keys)
    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
            case 72: return InputAction::ROTATE_CW;      // Up arrow
            case 80: return InputAction::MOVE_DOWN;      // Down arrow
            case 75: return InputAction::MOVE_LEFT;      // Left arrow
            case 77: return InputAction::MOVE_RIGHT;     // Right arrow
            default: return InputAction::NONE;
        }
    }

    // Handle regular keys
    switch (ch) {
        case ' ':  return InputAction::HARD_DROP;
        case 13:   return InputAction::START;            // Enter
        case 'z':
        case 'Z':  return InputAction::ROTATE_CW;
        case 'x':
        case 'X':  return InputAction::ROTATE_CCW;
        case 'p':
        case 'P':  return InputAction::PAUSE;
        case 'q':
        case 'Q':  return InputAction::QUIT;
        case 'r':
        case 'R':  return InputAction::RESTART;
        case 'w':
        case 'W':  return InputAction::ROTATE_CW;
        case 's':
        case 'S':  return InputAction::MOVE_DOWN;
        case 'a':
        case 'A':  return InputAction::MOVE_LEFT;
        case 'd':
        case 'D':  return InputAction::MOVE_RIGHT;
        default:   return InputAction::NONE;
    }
#else
    int ch = getchar();

    // Handle escape sequences (arrow keys)
    if (ch == 27) {
        if (getchar() == '[') {
            switch (getchar()) {
                case 'A': return InputAction::ROTATE_CW;     // Up arrow
                case 'B': return InputAction::MOVE_DOWN;     // Down arrow
                case 'C': return InputAction::MOVE_RIGHT;    // Right arrow
                case 'D': return InputAction::MOVE_LEFT;     // Left arrow
            }
        }
        return InputAction::NONE;
    }

    switch (ch) {
        case ' ':  return InputAction::HARD_DROP;
        case '\n': return InputAction::START;
        case 'z':
        case 'Z':  return InputAction::ROTATE_CW;
        case 'x':
        case 'X':  return InputAction::ROTATE_CCW;
        case 'p':
        case 'P':  return InputAction::PAUSE;
        case 'q':
        case 'Q':  return InputAction::QUIT;
        case 'r':
        case 'R':  return InputAction::RESTART;
        case 'w':
        case 'W':  return InputAction::ROTATE_CW;
        case 's':
        case 'S':  return InputAction::MOVE_DOWN;
        case 'a':
        case 'A':  return InputAction::MOVE_LEFT;
        case 'd':
        case 'D':  return InputAction::MOVE_RIGHT;
        default:   return InputAction::NONE;
    }
#endif
}
