#include "engine/generator/seven_bag_piece_generator.hpp"
#include <algorithm>
#include <random>

void SevenBagPieceGenerator::refillBag() {
    // Fill bag with one of each piece type
    this->bag = {
        TetrominoType::I,
        TetrominoType::O,
        TetrominoType::T,
        TetrominoType::S,
        TetrominoType::Z,
        TetrominoType::J,
        TetrominoType::L
    };

    // Shuffle the bag
    std::shuffle(this->bag.begin(), this->bag.end(), this->rng);
    this->bagIndex = 0;
}

TetrominoType SevenBagPieceGenerator::getNextFromBag() {
    // Refill bag if empty
    if (this->bagIndex >= 7) {
        this->refillBag();
    }

    return this->bag[this->bagIndex++];
}

SevenBagPieceGenerator::SevenBagPieceGenerator() : bagIndex(7) {
    // Initialize random number generator
    std::random_device rd;
    this->rng = std::mt19937(rd());

    // Fill initial preview
    for (int i = 0; i < 2; i++) {
        this->preview[i] = this->getNextFromBag();
    }
}

Tetromino SevenBagPieceGenerator::getNext(int SPAWN_X, int SPAWN_Y) {
    // Get the first piece from preview
    TetrominoType nextType = this->preview[0];

    // Shift preview and add new piece at the end
    this->preview[0] = this->preview[1];
    this->preview[1] = this->getNextFromBag();

    // Create tetromino at spawn position
    return Tetromino(nextType, SPAWN_X, SPAWN_Y);
}

std::array<TetrominoType, 2> SevenBagPieceGenerator::getPreview() const {
    return this->preview;
}
