#ifndef TETROMINO_H
#define TETROMINO_H

#include <vector>
#include <array>

enum class TetrominoType {
    I, O, T, S, Z, J, L, NONE
};

class Tetromino {
public:
    static const int MATRIX_SIZE = 4;

    Tetromino();
    explicit Tetromino(TetrominoType type);

    void rotate();
    void rotateCounterClockwise();

    TetrominoType getType() const;
    int getRotationState() const;
    const std::array<std::array<int, MATRIX_SIZE>, MATRIX_SIZE>& getShape() const;
    char getDisplayChar() const;

    static Tetromino createRandom();

private:
    TetrominoType type;
    int rotationState;
    std::array<std::array<int, MATRIX_SIZE>, MATRIX_SIZE> shape;

    void initializeShape();
    void applyRotation();

    static const std::vector<std::array<std::array<int, MATRIX_SIZE>, MATRIX_SIZE>>& getShapeDefinitions(TetrominoType type);
};

#endif
