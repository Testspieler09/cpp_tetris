#pragma once

#include "igame_engine.hpp"
#include "tetromino.hpp"
#include <array>
#include <random>

class PieceGenerator {
private:
    std::array<TetrominoType, 7> bag;
    int bagIndex;
    std::array<TetrominoType, 2> preview;
    std::mt19937 rng;

    void refillBag();
    TetrominoType getNextFromBag();

public:
    PieceGenerator();

    Tetromino getNext();
    std::array<TetrominoType, 2> getPreview() const { return preview; }
};
