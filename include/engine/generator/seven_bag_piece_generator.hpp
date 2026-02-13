#pragma once

#include "engine/generator/ipiece_generator.hpp"
#include "engine/core/tetromino.hpp"
#include <array>
#include <random>

class SevenBagPieceGenerator : public IPieceGenerator {
private:
    std::array<TetrominoType, 7> bag;
    int bagIndex;
    std::array<TetrominoType, 2> preview;
    std::mt19937 rng;

    void refillBag();
    TetrominoType getNextFromBag();

public:
    SevenBagPieceGenerator();

    Tetromino getNext(int SPAWN_X, int SPAWN_Y) override;
    std::array<TetrominoType, 2> getPreview() const override;
};
