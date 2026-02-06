#pragma once
#include "engine/igame_engine.hpp"
#include "engine/tetromino.hpp"
#include <raylib.h>
#include <map>

class Renderer {
private:
    IGameEngine& gameEngine;
    int screenWidth;
    int screenHeight;
    int cellSize;

    // Layout offsets
    int boardOffsetX;
    int boardOffsetY;
    int holdBoxX;
    int holdBoxY;
    int nextBoxX;
    int nextBoxY;

    // Input mapping (configurable)
    std::map<int, GameEvent> keyMapping;

    // Frame timing for 60 ticks per second
    static constexpr float TARGET_TICK_RATE = 1.0f / 60.0f;
    float tickAccumulator;

    // Helper rendering methods
    Color getColorForType(TetrominoType type) const;
    void drawCell(int gridX, int gridY, TetrominoType type, float alpha = 1.0f);
    void drawPieceShape(const int shape[4][4], int offsetX, int offsetY, TetrominoType type, float alpha = 1.0f);
    void drawTetromino(const GameState& state);
    void drawGhostPiece(const GameState& state);
    void drawBoard(const GameState& state);
    void drawHoldBox(const GameState& state);
    void drawNextBox(const GameState& state);
    void drawUI(const GameState& state);
    void drawGameOver();

    // Input handling
    void processInput();
    void setupDefaultKeyMapping();

public:
    Renderer(IGameEngine& game, int width = 800, int height = 600, int cellSize = 30);
    ~Renderer();

    // Main game loop
    void run();

    // Input configuration
    void mapKey(int raylibKey, GameEvent event);
    void clearKeyMapping();
};
