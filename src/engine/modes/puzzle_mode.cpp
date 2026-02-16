#include "engine/modes/puzzle_mode.hpp"
#include "engine/core/game.hpp"
#include "engine/core/game_config.hpp"
#include "engine/core/igame_engine.hpp"

struct PuzzleLevel {
    std::vector<std::vector<char>> initialBoard;
    std::vector<TetrominoType> pieceSequence;
};

// TODO: create some nice puzzles
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
            {8,8,8,0,8,8,8,8,8,8},
            {8,8,8,0,8,8,8,8,8,8},
            {8,8,8,0,8,8,8,8,8,8},
            {8,8,8,0,8,8,8,8,8,8}
        },
        {
            TetrominoType::I
        }
    }
};

bool PuzzleMode::advancePuzzle(Game& game) {
    if (!game.isBoardEmpty()) {
        return false;
    }

    this->currentPuzzleIndex++;

    if (this->currentPuzzleIndex >= PUZZLES.size()) {
        return false;
    }

    const PuzzleLevel& puzzle = PUZZLES[this->currentPuzzleIndex];

    this->generator->setLevelPieces(puzzle.pieceSequence);
    game.setBoardTo(puzzle.initialBoard);

    return true;
}

void PuzzleMode::reset() {
    if (!(this->currentPuzzleIndex >= PUZZLES.size())) {
        return;
    }
    this->currentPuzzleIndex = 0;

    const PuzzleLevel& puzzle = PUZZLES[this->currentPuzzleIndex];
    this->generator->setLevelPieces(puzzle.pieceSequence);
}

GameConfig PuzzleMode::getInitialConfig() const {
    const PuzzleLevel& puzzle = PUZZLES[this->currentPuzzleIndex];

    this->generator->setLevelPieces(puzzle.pieceSequence);

    GameConfig cfg;

    cfg.canHoldAnything = false;
    cfg.initialBoard = puzzle.initialBoard;
    cfg.startingScore = this->score;

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
    if (boardEmpty && this->currentPuzzleIndex == PUZZLES.size()) {
        return true;
    } else {
        return false;
    }
};

bool PuzzleMode::checkLose(const Game& game) const {
    bool noMorePieces = !this->generator->hasMorePieces();
    bool boardNotEmpty = !game.isBoardEmpty();
    bool hitTop = game.hasHitTopOfBoard();

    return hitTop || (noMorePieces && boardNotEmpty);
};
