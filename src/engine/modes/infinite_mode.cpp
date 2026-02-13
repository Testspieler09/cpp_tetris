#include "engine/core/game.hpp"
#include "engine/core/game_config.hpp"
#include "engine/core/igame_engine.hpp"
#include "engine/core/tetromino.hpp"
#include <array>
#include "engine/modes/infinite_mode.hpp"

GameConfig InfiniteMode::getInitialConfig() const { return {}; }
Tetromino InfiniteMode::getNextPiece(int SPAWN_X, int SPAWN_Y)  { return this->generator->getNext(SPAWN_X, SPAWN_Y); }
std::array<TetrominoType, 2> InfiniteMode::getPiecePreview() const { return this->generator->getPreview(); }

bool InfiniteMode::checkWin(const Game& /* ignored */) const { return false; }
bool InfiniteMode::checkLose(const Game& game) const { return game.hasHitTopOfBoard(); }
