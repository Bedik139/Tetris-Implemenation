#include "../../include/Model/Tetromino.h"
#include <cstdlib>
#include <ctime>

// Shape definitions for all 7 Tetrominoes with 4 rotation states each
// 1 = filled cell, 0 = empty

// I-piece rotations
static const std::vector<std::array<std::array<int, 4>, 4>> I_SHAPES = {{
    {{{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}},
    {{{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}}},
    {{{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}}},
    {{{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}}
}};

// O-piece rotations (all same)
static const std::vector<std::array<std::array<int, 4>, 4>> O_SHAPES = {{
    {{{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}}}
}};

// T-piece rotations
static const std::vector<std::array<std::array<int, 4>, 4>> T_SHAPES = {{
    {{{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,1,0,0}}},
    {{{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,1,0,0}}},
    {{{0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {0,1,0,0}}}
}};

// S-piece rotations
static const std::vector<std::array<std::array<int, 4>, 4>> S_SHAPES = {{
    {{{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,1,0}}},
    {{{0,0,0,0}, {0,0,0,0}, {0,1,1,0}, {1,1,0,0}}},
    {{{0,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,1,0,0}}}
}};

// Z-piece rotations
static const std::vector<std::array<std::array<int, 4>, 4>> Z_SHAPES = {{
    {{{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,0,1,0}, {0,1,1,0}, {0,1,0,0}}},
    {{{0,0,0,0}, {0,0,0,0}, {1,1,0,0}, {0,1,1,0}}},
    {{{0,0,0,0}, {0,1,0,0}, {1,1,0,0}, {1,0,0,0}}}
}};

// J-piece rotations
static const std::vector<std::array<std::array<int, 4>, 4>> J_SHAPES = {{
    {{{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,1,1,0}, {0,1,0,0}, {0,1,0,0}}},
    {{{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {0,0,1,0}}},
    {{{0,0,0,0}, {0,1,0,0}, {0,1,0,0}, {1,1,0,0}}}
}};

// L-piece rotations
static const std::vector<std::array<std::array<int, 4>, 4>> L_SHAPES = {{
    {{{0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0}}},
    {{{0,0,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,1,0}}},
    {{{0,0,0,0}, {0,0,0,0}, {1,1,1,0}, {1,0,0,0}}},
    {{{0,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,0,0}}}
}};

static bool randomSeeded = false;

Tetromino::Tetromino() : type(TetrominoType::NONE), rotationState(0) {
    for (auto& row : shape) {
        row.fill(0);
    }
}

Tetromino::Tetromino(TetrominoType type) : type(type), rotationState(0) {
    initializeShape();
}

void Tetromino::rotate() {
    rotationState = (rotationState + 1) % 4;
    applyRotation();
}

void Tetromino::rotateCounterClockwise() {
    rotationState = (rotationState + 3) % 4;
    applyRotation();
}

TetrominoType Tetromino::getType() const {
    return type;
}

int Tetromino::getRotationState() const {
    return rotationState;
}

const std::array<std::array<int, Tetromino::MATRIX_SIZE>, Tetromino::MATRIX_SIZE>& Tetromino::getShape() const {
    return shape;
}

char Tetromino::getDisplayChar() const {
    switch (type) {
        case TetrominoType::I: return 'I';
        case TetrominoType::O: return 'O';
        case TetrominoType::T: return 'T';
        case TetrominoType::S: return 'S';
        case TetrominoType::Z: return 'Z';
        case TetrominoType::J: return 'J';
        case TetrominoType::L: return 'L';
        default: return ' ';
    }
}

Tetromino Tetromino::createRandom() {
    if (!randomSeeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        randomSeeded = true;
    }
    int randomType = rand() % 7;
    return Tetromino(static_cast<TetrominoType>(randomType));
}

void Tetromino::initializeShape() {
    applyRotation();
}

void Tetromino::applyRotation() {
    const auto& shapes = getShapeDefinitions(type);
    if (!shapes.empty()) {
        shape = shapes[rotationState];
    }
}

const std::vector<std::array<std::array<int, Tetromino::MATRIX_SIZE>, Tetromino::MATRIX_SIZE>>&
Tetromino::getShapeDefinitions(TetrominoType type) {
    switch (type) {
        case TetrominoType::I: return I_SHAPES;
        case TetrominoType::O: return O_SHAPES;
        case TetrominoType::T: return T_SHAPES;
        case TetrominoType::S: return S_SHAPES;
        case TetrominoType::Z: return Z_SHAPES;
        case TetrominoType::J: return J_SHAPES;
        case TetrominoType::L: return L_SHAPES;
        default: {
            static std::vector<std::array<std::array<int, MATRIX_SIZE>, MATRIX_SIZE>> empty;
            return empty;
        }
    }
}
