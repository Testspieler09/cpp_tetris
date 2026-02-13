#pragma once

#include "engine/generator/ipiece_generator.hpp"
#include "engine/core/igame_engine.hpp"
#include "engine/core/tetromino.hpp"

class FixedSequenceGenerator: public IPieceGenerator {
private:
    std::array<TetrominoType, 2> preview;
    TetrominoType getNextFromLevel();

public:
    FixedSequenceGenerator() = default;

    Tetromino getNext(const int SPAWN_X, const int SPAWN_Y) override;
    std::array<TetrominoType, 2> getPreview() const override;
};
