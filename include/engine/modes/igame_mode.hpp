#pragma once

#include <array>
#include <memory>

#include "engine/core/igame_engine.hpp"
#include "engine/generator/ipiece_generator.hpp"
#include "engine/core/game_config.hpp"

class Game;
class PieceGenerator;

class IGameMode {
private:
    std::unique_ptr<IPieceGenerator> generator;

public:
    virtual ~IGameMode() = default;

    virtual GameConfig getInitialConfig() const = 0;

    virtual Tetromino getNextPiece(const int SPAWN_X, const int SPAWN_Y) = 0;
    virtual std::array<TetrominoType, 2> getPiecePreview() const = 0;

    virtual bool checkWin(const Game& game) const = 0;
    virtual bool checkLose(const Game& game) const = 0;

    // Will return `true` if it was successfull otherwise `false`
    virtual bool advancePuzzle(Game& /* ignored */) { return false; }
    virtual void reset() = 0;
};
