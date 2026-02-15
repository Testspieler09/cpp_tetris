#include "engine/modes/puzzle_mode.hpp"
#include "engine/core/game.hpp"
#include "engine/core/game_config.hpp"
#include <iostream>

struct PuzzleLevel {
    std::vector<std::vector<char>> initialBoard;
    std::vector<TetrominoType> pieceSequence;
};

static const std::vector<PuzzleLevel> PUZZLES = {
    {
        // Puzzle 1
        {
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {8,8,0,0,0,8,8,8,8,8},
            {8,8,8,0,8,8,8,8,8,8}
        },
        {
            TetrominoType::T
        }
    },
    {
        // Puzzle 2
        {
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0},
            {8,8,8,0,8,8,8,8,8,8}
        },
        {
            TetrominoType::J,
            TetrominoType::L,
            TetrominoType::S
        }
    }
};

bool PuzzleMode::advancePuzzle(Game& game) {
    if (!game.isBoardEmpty()) {
        return true;
    }

    this->currentPuzzleIndex++;

    if (this->currentPuzzleIndex >= PUZZLES.size()) {
        this->currentPuzzleIndex = 0; // For restart to work
        return false;
    }

    const PuzzleLevel& puzzle = PUZZLES[this->currentPuzzleIndex];

    this->generator->setLevelPieces(puzzle.pieceSequence);
    game.setBoardTo(puzzle.initialBoard);

    return true;
}

GameConfig PuzzleMode::getInitialConfig() const {
    const PuzzleLevel& puzzle = PUZZLES[this->currentPuzzleIndex];

    this->generator->setLevelPieces(puzzle.pieceSequence);

    GameConfig cfg;

    cfg.canHoldAnything = false;
    cfg.initialBoard = puzzle.initialBoard;

    // TODO: set the score based on this->score and provide an update fn or similar

    return cfg;
};

Tetromino PuzzleMode::getNextPiece(const int SPAWN_X, const int SPAWN_Y) {
    return this->generator->getNext(SPAWN_X, SPAWN_Y);
};

std::array<TetrominoType, 2> PuzzleMode::getPiecePreview() const {
    return this->generator->getPreview();
}

bool PuzzleMode::checkWin(const Game& game) const {
    bool boardEmpty = game.isBoardEmpty();
    if (boardEmpty && this->currentPuzzleIndex == PUZZLES.size() - 1) {
        return true;
    } else {
        return false;
    }
};

bool PuzzleMode::checkLose(const Game& game) const {
    bool noMorePieces = !this->generator->hasMorePieces();
    bool boardNotEmpty = !game.isBoardEmpty();
    bool hitTop = game.hasHitTopOfBoard();

    // FIX: 101 which is wrong it should be 010
    std::cout << hitTop << noMorePieces << boardNotEmpty << std::endl;

    return hitTop || (noMorePieces && boardNotEmpty);
};
