#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

/*
 * Tetris Application
*/
class TetrisApp {
    public:
        // Constructor
        TetrisApp();

        void run(){
            std::cout << "Running Tetris Application..." << std::endl;
            std::cout << "Welcome to Tetris!" << std::endl;
        };

    private:
        int score;
        int level;
};