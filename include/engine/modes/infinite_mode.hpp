#pragma once

#include "engine/core/igame_engine.hpp"
#include "engine/modes/igame_mode.hpp"
#include "engine/generator/seven_bag_piece_generator.hpp"
#include <memory>

class InfiniteMode : public IGameMode {
private:
    std::unique_ptr<IPieceGenerator> generator;

public:
    InfiniteMode()
        : generator(std::make_unique<SevenBagPieceGenerator>())
    {}

    GameConfig getInitialConfig() const override;
    Tetromino getNextPiece(int SPAWN_X, int SPAWN_Y) override;
    std::array<TetrominoType, 2> getPiecePreview() const override;

    bool checkWin(const Game& /* ignored */) const override;
    bool checkLose(const Game& game) const override;
};
