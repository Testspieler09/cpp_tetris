#pragma once

#include "engine/core/igame_engine.hpp"
#include "engine/generator/ipiece_generator.hpp"
#include "engine/modes/igame_mode.hpp"
#include <memory>

class InfiniteMode : public IGameMode {
private:
    std::unique_ptr<IPieceGenerator> generator;

public:
    InfiniteMode(std::unique_ptr<IPieceGenerator> generator) { this->generator = std::move(generator); }

    GameConfig getInitialConfig() const override;
    Tetromino getNextPiece(int SPAWN_X, int SPAWN_Y) override;
    std::array<TetrominoType, 2> getPiecePreview() const override;

    bool checkWin(const Game& /* ignored */) const override;
    bool checkLose(const Game& game) const override;
};
