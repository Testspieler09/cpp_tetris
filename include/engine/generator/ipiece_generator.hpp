#pragma once

#include "engine/core/igame_engine.hpp"
#include "engine/core/tetromino.hpp"
#include <array>

class IPieceGenerator {
public:
    virtual ~IPieceGenerator() = default;

    virtual Tetromino getNext(const int SPAWN_X, const int SPAWN_Y) = 0;
    virtual std::array<TetrominoType, 2> getPreview() const = 0;
};
