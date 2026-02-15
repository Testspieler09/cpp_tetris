#pragma once

#include "engine/core/igame_engine.hpp"
#include "engine/modes/igame_mode.hpp"
#include "tetromino.hpp"
#include <optional>
#include <vector>

class Game : public IGameEngine {
private:
    static constexpr int SPAWN_X = 3;
    static constexpr int SPAWN_Y = 0;

    std::unique_ptr<IGameMode> mode;

    // Board state
    int board_width;
    int board_height;
    std::vector<std::vector<char>> board;

    // Game state
    Tetromino currentPiece;
    std::optional<Tetromino> heldPiece;
    bool canHold;
    bool canHoldAnything;

    // Game stats
    int score;
    int level;
    int linesCleared;
    bool gameOver;
    bool won;

    // Timing
    float dropTimer;
    float dropInterval;

    // Private game logic methods
    bool isValidPosition(const Tetromino& piece) const;
    bool isValidPosition(const Tetromino& piece, int offsetX, int offsetY) const;
    void lockPiece();
    int clearLines();
    void spawnNextPiece();
    int calculateGhostY() const;
    void updateDropInterval();

    // Movement helpers (return true if successful)
    bool tryMoveLeft();
    bool tryMoveRight();
    bool tryMoveDown();
    bool tryRotate(bool clockwise);
    void performHardDrop();
    void performHold();

public:
    Game(std::unique_ptr<IGameMode> mode);

    // IGameEngine interface implementation
    void update(float deltaTime) override;
    void handleEvent(GameEvent event) override;
    GameState getState() const override;

    // Getter for IGameMode
    bool hasHitTopOfBoard() const {
        return !this->isValidPosition(this->currentPiece);
    }
    bool isBoardEmpty() const {
        return std::all_of(
            board.begin(),
            board.end(),
            [](const std::vector<char>& row) {
                return std::all_of(
                    row.begin(),
                    row.end(),
                    [](char c) { return c == 0; }
                );
            }
        );
    }
    void setBoardTo(std::vector<std::vector<char>> new_board) {
        this->board = new_board;
    }

    // Reset game
    void reset();
};
