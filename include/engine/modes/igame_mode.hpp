#pragma once

#include "engine/core/igame_engine.hpp"
#include "engine/generator/ipiece_generator.hpp"
#include "engine/core/game_config.hpp"
#include <array>
#include <memory>

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
};
