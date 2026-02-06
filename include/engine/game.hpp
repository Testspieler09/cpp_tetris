#pragma once

#include "igame_engine.hpp"
#include "tetromino.hpp"
#include "piece_generator.hpp"
#include <optional>

class Game : public IGameEngine {
private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;
    static constexpr int SPAWN_X = 3;
    static constexpr int SPAWN_Y = 0;

    // Board state
    int board[BOARD_HEIGHT][BOARD_WIDTH];

    // Game state
    Tetromino currentPiece;
    std::optional<Tetromino> heldPiece;
    bool canHold;
    PieceGenerator generator;

    // Game stats
    int score;
    int level;
    int linesCleared;
    bool gameOver;

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
    Game();

    // IGameEngine interface implementation
    void update(float deltaTime) override;
    void handleEvent(GameEvent event) override;
    GameState getState() const override;

    // Reset game
    void reset();
};
