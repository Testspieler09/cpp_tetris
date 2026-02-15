#include "engine/core/igame_engine.hpp"
#include "engine/generator/fixed_sequence_generator.hpp"
#include <array>
#include <vector>

void FixedSequenceGenerator::setLevelPieces(std::vector<TetrominoType> pieces) {
    this->level_pieces = std::move(pieces);
    this->currentPieceIndex = 0;

    if (this->level_pieces.size() > 0) {
        this->preview[0] = this->level_pieces[0];
    } else {
        this->preview[0] = TetrominoType::NONE;
    }

    if (this->level_pieces.size() > 1) {
        this->preview[1] = this->level_pieces[1];
    } else {
        this->preview[1] = TetrominoType::NONE;
    }
}

bool FixedSequenceGenerator::hasMorePieces() const {
    // + 1 because if there are no Tetromino left we need to let the user still place it
    return this->currentPieceIndex < this->level_pieces.size() + 1;
}

Tetromino FixedSequenceGenerator::getNext(int SPAWN_X, int SPAWN_Y) {
    if (this->currentPieceIndex >= this->level_pieces.size()) {
        return Tetromino(TetrominoType::NONE, SPAWN_X, SPAWN_Y);
    }

    TetrominoType nextType = this->level_pieces[this->currentPieceIndex];
    this->currentPieceIndex++;

    preview[0] = (currentPieceIndex < level_pieces.size())
        ? level_pieces[currentPieceIndex]
        : TetrominoType::NONE;
    preview[1] = (currentPieceIndex + 1 < level_pieces.size())
        ? level_pieces[currentPieceIndex + 1]
        : TetrominoType::NONE;

    return Tetromino(nextType, SPAWN_X, SPAWN_Y);
}

std::array<TetrominoType, 2> FixedSequenceGenerator::getPreview() const {
    return this->preview;
}
