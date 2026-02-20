#include "engine/modes/puzzle_mode.hpp"
#include "engine/core/game.hpp"
#include "engine/core/game_config.hpp"
#include "engine/core/igame_engine.hpp"

struct PuzzleLevel {
    std::vector<std::vector<char>> initialBoard;
    std::vector<TetrominoType> pieceSequence;
};

static const std::vector<PuzzleLevel> PUZZLES = {
    {
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
            {8,8,0,0,0,0,8,8,8,8},
            {8,8,8,0,0,0,0,8,8,8}
        },
        {
            TetrominoType::I,
            TetrominoType::I
        }
    },
    {
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
            {8,0,0,0,8,8,0,0,0,8},
            {8,8,0,0,8,8,0,0,8,8},
            {8,8,0,0,8,8,0,0,8,8},
            {8,8,0,8,8,8,8,0,8,8}
        },
        {
            TetrominoType::T,
            TetrominoType::S,
            TetrominoType::T,
            TetrominoType::J
        }
    },
    {
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
            {0,8,8,0,0,0,0,0,0,0},
            {0,8,8,0,0,0,0,0,0,0}
        },
        {
            TetrominoType::T,
            TetrominoType::L,
            TetrominoType::Z,
            TetrominoType::T,
            TetrominoType::S,
            TetrominoType::J,
            TetrominoType::O,
            TetrominoType::I,
            TetrominoType::I
        }
    },
    {
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
            {0,0,0,8,0,0,0,0,0,0},
            {8,8,0,8,8,8,0,0,8,0},
            {8,8,0,8,8,8,8,8,8,8}
        },
        {
            TetrominoType::J,
            TetrominoType::Z,
            TetrominoType::T,
            TetrominoType::O,
            TetrominoType::L,
            TetrominoType::I
        }
    },
    {
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
            {0,0,0,8,0,0,0,0,0,0},
            {8,8,0,8,8,8,0,0,8,0},
            {8,8,0,8,8,8,8,8,8,8}
        },
        {
            TetrominoType::O,
            TetrominoType::Z,
            TetrominoType::L,
            TetrominoType::T,
            TetrominoType::J,
            TetrominoType::I
        }
    },
    {
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
            {8,0,0,0,0,0,0,0,0,0},
            {8,0,0,0,0,0,0,0,8,8},
            {8,8,8,8,0,0,0,0,8,8},
            {8,8,8,8,8,0,0,8,8,8},
            {8,8,8,8,8,0,0,8,8,8}
        },
        {
            TetrominoType::O,
            TetrominoType::S,
            TetrominoType::L,
            TetrominoType::Z,
            TetrominoType::T,
            TetrominoType::J
        }
    },
    {
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
            {0,8,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,8,8,8,0,0}
        },
        {
            TetrominoType::I,
            TetrominoType::L,
            TetrominoType::O,
            TetrominoType::T,
            TetrominoType::S,
            TetrominoType::L,
            TetrominoType::I,
            TetrominoType::T,
            TetrominoType::I
        }
    }
};

bool PuzzleMode::advancePuzzle(Game& game) {
    if (!game.isBoardEmpty()) {
        return false;
    }

    this->score = game.getCurrentScore();
    this->currentPuzzleIndex++;

    if (this->currentPuzzleIndex >= PUZZLES.size()) {
        this->completed = true;
        return false;
    }

    const PuzzleLevel& puzzle = PUZZLES[this->currentPuzzleIndex];

    this->generator->setLevelPieces(puzzle.pieceSequence);
    game.setBoardTo(puzzle.initialBoard);

    return true;
}

void PuzzleMode::reset() {
    if (this->completed) {
        this->currentPuzzleIndex = 0;
        this->score = 0;
        this->completed = false;
    }

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
