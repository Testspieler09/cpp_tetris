#include "engine/modes/puzzle_mode.hpp"
#include "engine/core/game.hpp"
#include "engine/core/game_config.hpp"

// Levels levels;
// or use consts

GameConfig PuzzleMode::getInitialConfig() const {
    GameConfig cfg;

    // FIX: this is true after first tick. Introduce a gloabl flag or similar
    cfg.canHold = false;

    return cfg;
};
Tetromino PuzzleMode::getNextPiece(const int SPAWN_X, const int SPAWN_Y) { return generator->getNext(SPAWN_X, SPAWN_Y); };
std::array<TetrominoType, 2> PuzzleMode::getPiecePreview() const { return this->generator->getPreview(); }

// TODO: add the logic
bool PuzzleMode::checkWin(const Game& game) const { return false; };
bool PuzzleMode::checkLose(const Game& game) const {
    return /* other condition || */ game.hasHitTopOfBoard();
};
