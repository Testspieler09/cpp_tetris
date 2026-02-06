#include "engine/piece_generator.hpp"
#include <algorithm>
#include <random>

PieceGenerator::PieceGenerator() : bagIndex(7) {
    // Initialize random number generator
    std::random_device rd;
    rng = std::mt19937(rd());

    // Fill initial preview
    for (int i = 0; i < 2; i++) {
        preview[i] = getNextFromBag();
    }
}

void PieceGenerator::refillBag() {
    // Fill bag with one of each piece type
    bag = {
        TetrominoType::I,
        TetrominoType::O,
        TetrominoType::T,
        TetrominoType::S,
        TetrominoType::Z,
        TetrominoType::J,
        TetrominoType::L
    };

    // Shuffle the bag
    std::shuffle(bag.begin(), bag.end(), rng);
    bagIndex = 0;
}

TetrominoType PieceGenerator::getNextFromBag() {
    // Refill bag if empty
    if (bagIndex >= 7) {
        refillBag();
    }

    return bag[bagIndex++];
}

Tetromino PieceGenerator::getNext() {
    // Get the first piece from preview
    TetrominoType nextType = preview[0];

    // Shift preview and add new piece at the end
    preview[0] = preview[1];
    preview[1] = getNextFromBag();

    // Create tetromino at spawn position
    return Tetromino(nextType, 3, 0);
}
