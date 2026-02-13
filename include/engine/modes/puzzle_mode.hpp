#pragma once

#include "engine/core/game_config.hpp"
#include "engine/modes/igame_mode.hpp"
#include "engine/generator/fixed_sequence_generator.hpp"

class PuzzleMode : public IGameMode {
private:
    std::unique_ptr<IPieceGenerator> generator;

public:
    PuzzleMode()
        : generator(std::make_unique<FixedSequenceGenerator>())
    {}

    GameConfig getInitialConfig() const override;
    Tetromino getNextPiece(const int SPAWN_X, const int SPAWN_Y) override;
    std::array<TetrominoType, 2> getPiecePreview() const override;

    bool checkWin(const Game& game) const override;
    bool checkLose(const Game& game) const override;
};
