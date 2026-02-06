#include "engine/tetromino.hpp"
#include <cstring>

// Base shapes for each tetromino type at each orientation
// 0 = empty, 1 = filled
static const int SHAPES[8][4][4][4] = {
    // NONE
    {
        {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}
    },
    // I piece
    {
        // North
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        // East
        {{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}},
        // South
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}},
        // West
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}
    },
    // O piece
    {
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}
    },
    // T piece
    {
        // North
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        // East
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        // South
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        // West
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // S piece
    {
        // North
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        // East
        {{0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        // South
        {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
        // West
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    },
    // Z piece
    {
        // North
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        // East
        {{0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}},
        // South
        {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        // West
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}
    },
    // J piece
    {
        // North
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        // East
        {{0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},
        // South
        {{0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0}},
        // West
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}}
    },
    // L piece
    {
        // North
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        // East
        {{0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0}},
        // South
        {{0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0}},
        // West
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}}
    }
};

Tetromino::Tetromino()
    : type(TetrominoType::NONE), orientation(Orientation::NORTH), x(0), y(0) {
    std::memset(this->shape, 0, sizeof(this->shape));
}

Tetromino::Tetromino(TetrominoType type, int startX, int startY)
    : type(type), orientation(Orientation::NORTH), x(startX), y(startY) {
    updateShape();
}

void Tetromino::updateShape() {
    getBaseShape(this->type, this->orientation, this->shape);
}

void Tetromino::getShape(int outShape[4][4]) const {
    std::memcpy(outShape, this->shape, sizeof(int) * 16);
}

void Tetromino::setOrientation(Orientation newOrientation) {
    this->orientation = newOrientation;
    updateShape();
}

void Tetromino::getBaseShape(TetrominoType type, Orientation orientation, int outShape[4][4]) {
    int typeIndex = static_cast<int>(type);
    int orientIndex = static_cast<int>(orientation);
    std::memcpy(outShape, SHAPES[typeIndex][orientIndex], sizeof(int) * 16);
}
