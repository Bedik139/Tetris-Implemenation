#include <iostream>
#include <vector>

class TetrisApp {
public:
    // Constructor
    TetrisApp() {
        score = 0;
        level = 1;
    }

    // A method to start the game
    void run() {
        std::cout << "Tetris Started! Level: " << level << " Score: " << score << std::endl;
    }

private:
    int score;
    int level;
};


int main(int argc, char* argv[]) {
    // Safety check for arguments
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1; 
    }

    std::cout << "Loading file: " << argv[1] << std::endl;

    TetrisApp myGame;
    myGame.run();

    return 0; 
}