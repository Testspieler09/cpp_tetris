#pragma once

#include "igame_engine.hpp"

class Tetromino {
private:
    TetrominoType type;
    Orientation orientation;
    int x, y;
    int shape[4][4];

    void updateShape();

public:
    Tetromino();
    Tetromino(TetrominoType type, int startX, int startY);

    // Getters
    TetrominoType getType() const { return type; }
    Orientation getOrientation() const { return orientation; }
    int getX() const { return x; }
    int getY() const { return y; }
    void getShape(int outShape[4][4]) const;

    // Movement
    void moveLeft() { x--; }
    void moveRight() { x++; }
    void moveDown() { y++; }
    void setPosition(int newX, int newY) { x = newX; y = newY; }

    // Rotation
    void setOrientation(Orientation newOrientation);

    // Get base shape for a tetromino type at specific orientation
    static void getBaseShape(TetrominoType type, Orientation orientation, int outShape[4][4]);
};
