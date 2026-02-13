#include "engine/core/igame_engine.hpp"
#include "engine/generator/fixed_sequence_generator.hpp"
#include <array>

Tetromino FixedSequenceGenerator::getNext(int SPAWN_X, int SPAWN_Y) {
    // Get the first piece from preview
    TetrominoType nextType = this->preview[0];

    // Shift preview and add new piece at the end
    this->preview[0] = this->preview[1];
    this->preview[1] = this->getNextFromLevel();

    // Create tetromino at spawn position
    return Tetromino(nextType, SPAWN_X, SPAWN_Y);
}

std::array<TetrominoType, 2> FixedSequenceGenerator::getPreview() const {
    return this->preview;
}
