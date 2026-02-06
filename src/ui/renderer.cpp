#include "ui/renderer.hpp"
#include "raylib.h"
#include <cstring>

Renderer::Renderer(IGameEngine& game, int width, int height, int cellSize)
    : gameEngine(game), screenWidth(width), screenHeight(height),
      cellSize(cellSize), tickAccumulator(0.0f) {

    // Calculate layout
    boardOffsetX = 250;
    boardOffsetY = 50;

    holdBoxX = 50;
    holdBoxY = 50;

    nextBoxX = boardOffsetX + (10 * cellSize) + 50;
    nextBoxY = 50;

    // Initialize window
    InitWindow(screenWidth, screenHeight, "Tetris - SRS");
    SetTargetFPS(60);

    setupDefaultKeyMapping();
}

Renderer::~Renderer() {
    CloseWindow();
}

void Renderer::setupDefaultKeyMapping() {
    mapKey(KEY_LEFT, GameEvent::MOVE_LEFT);
    mapKey(KEY_RIGHT, GameEvent::MOVE_RIGHT);
    mapKey(KEY_DOWN, GameEvent::MOVE_DOWN);
    mapKey(KEY_UP, GameEvent::ROTATE_CW);
    mapKey(KEY_Z, GameEvent::ROTATE_CCW);
    mapKey(KEY_X, GameEvent::ROTATE_CW);
    mapKey(KEY_SPACE, GameEvent::HARD_DROP);
    mapKey(KEY_C, GameEvent::HOLD);
    mapKey(KEY_LEFT_SHIFT, GameEvent::HOLD);
    mapKey(KEY_R, GameEvent::RESTART);
}

void Renderer::mapKey(int raylibKey, GameEvent event) {
    keyMapping[raylibKey] = event;
}

void Renderer::clearKeyMapping() {
    keyMapping.clear();
}

void Renderer::run() {
    while (!WindowShouldClose()) {
        float frameTime = GetFrameTime();

        // Process input
        processInput();

        // Fixed timestep update (60 ticks per second)
        tickAccumulator += frameTime;
        while (tickAccumulator >= TARGET_TICK_RATE) {
            gameEngine.update(TARGET_TICK_RATE);
            tickAccumulator -= TARGET_TICK_RATE;
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        GameState state = gameEngine.getState();

        drawBoard(state);
        drawGhostPiece(state);
        drawTetromino(state);
        drawHoldBox(state);
        drawNextBox(state);
        drawUI(state);

        if (state.gameOver) {
            drawGameOver();
        }

        EndDrawing();
    }
}

void Renderer::processInput() {
    for (const auto& mapping : keyMapping) {
        if (IsKeyPressed(mapping.first)) {
            gameEngine.handleEvent(mapping.second);
        }
    }
}

Color Renderer::getColorForType(TetrominoType type) const {
    switch (type) {
        case TetrominoType::I: return {0, 255, 255, 255};    // Cyan
        case TetrominoType::O: return {255, 255, 0, 255};    // Yellow
        case TetrominoType::T: return {128, 0, 128, 255};    // Purple
        case TetrominoType::S: return {0, 255, 0, 255};      // Green
        case TetrominoType::Z: return {255, 0, 0, 255};      // Red
        case TetrominoType::J: return {0, 0, 255, 255};      // Blue
        case TetrominoType::L: return {255, 165, 0, 255};    // Orange
        default: return {128, 128, 128, 255};                // Gray
    }
}

void Renderer::drawCell(int gridX, int gridY, TetrominoType type, float alpha) {
    Color color = getColorForType(type);
    color.a = static_cast<unsigned char>(255 * alpha);

    int x = boardOffsetX + gridX * cellSize;
    int y = boardOffsetY + gridY * cellSize;

    DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, color);
    DrawRectangleLines(x, y, cellSize, cellSize, WHITE);
}

void Renderer::drawPieceShape(const int shape[4][4], int offsetX, int offsetY,
                               TetrominoType type, float alpha) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] != 0) {
                Color color = getColorForType(type);
                color.a = static_cast<unsigned char>(255 * alpha);

                int x = offsetX + col * cellSize;
                int y = offsetY + row * cellSize;

                DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, color);
                DrawRectangleLines(x, y, cellSize, cellSize, WHITE);
            }
        }
    }
}

void Renderer::drawBoard(const GameState& state) {
    // Draw board background
    DrawRectangle(boardOffsetX, boardOffsetY,
                  10 * cellSize, 20 * cellSize,
                  {20, 20, 20, 255});

    // Draw grid and locked pieces
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
            int x = boardOffsetX + col * cellSize;
            int y = boardOffsetY + row * cellSize;

            DrawRectangleLines(x, y, cellSize, cellSize, {50, 50, 50, 255});

            if (state.board[row][col] != 0) {
                TetrominoType type = static_cast<TetrominoType>(state.board[row][col]);
                drawCell(col, row, type, 1.0f);
            }
        }
    }
}

void Renderer::drawTetromino(const GameState& state) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (state.currentPieceShape[row][col] != 0) {
                drawCell(
                    state.currentPieceX + col,
                    state.currentPieceY + row,
                    state.currentPieceType,
                    1.0f
                );
            }
        }
    }
}

void Renderer::drawGhostPiece(const GameState& state) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (state.currentPieceShape[row][col] != 0) {
                drawCell(
                    state.currentPieceX + col,
                    state.ghostPieceY + row,
                    state.currentPieceType,
                    0.3f
                );
            }
        }
    }
}

void Renderer::drawHoldBox(const GameState& state) {
    DrawText("HOLD", holdBoxX, holdBoxY - 25, 20, WHITE);

    // Draw hold box background
    DrawRectangle(holdBoxX, holdBoxY, 4 * cellSize, 4 * cellSize, {20, 20, 20, 255});
    DrawRectangleLines(holdBoxX, holdBoxY, 4 * cellSize, 4 * cellSize, WHITE);

    if (state.hasHeldPiece) {
        int heldShape[4][4];
        Tetromino::getBaseShape(state.heldPieceType, Orientation::NORTH, heldShape);

        float alpha = state.canHold ? 1.0f : 0.4f;
        drawPieceShape(heldShape, holdBoxX, holdBoxY, state.heldPieceType, alpha);
    }
}

void Renderer::drawNextBox(const GameState& state) {
    DrawText("NEXT", nextBoxX, nextBoxY - 25, 20, WHITE);

    int yOffset = nextBoxY;

    for (int i = 0; i < 2; i++) {
        // Draw next box background
        DrawRectangle(nextBoxX, yOffset, 4 * cellSize, 4 * cellSize, {20, 20, 20, 255});
        DrawRectangleLines(nextBoxX, yOffset, 4 * cellSize, 4 * cellSize, WHITE);

        if (state.nextPieces[i] != TetrominoType::NONE) {
            int nextShape[4][4];
            Tetromino::getBaseShape(state.nextPieces[i], Orientation::NORTH, nextShape);
            drawPieceShape(nextShape, nextBoxX, yOffset, state.nextPieces[i], 1.0f);
        }

        yOffset += 4 * cellSize + 20;
    }
}

void Renderer::drawUI(const GameState& state) {
    int uiX = nextBoxX;
    int uiY = nextBoxY + 2 * (4 * cellSize + 20) + 30;

    DrawText(TextFormat("SCORE: %d", state.score), uiX, uiY, 20, WHITE);
    DrawText(TextFormat("LEVEL: %d", state.level), uiX, uiY + 30, 20, WHITE);
    DrawText(TextFormat("LINES: %d", state.linesCleared), uiX, uiY + 60, 20, WHITE);

    // Controls
    int controlsY = screenHeight - 150;
    DrawText("CONTROLS:", 50, controlsY, 16, GRAY);
    DrawText("Arrows: Move", 50, controlsY + 25, 14, GRAY);
    DrawText("Up/X: Rotate CW", 50, controlsY + 45, 14, GRAY);
    DrawText("Z: Rotate CCW", 50, controlsY + 65, 14, GRAY);
    DrawText("Space: Hard Drop", 50, controlsY + 85, 14, GRAY);
    DrawText("C/Shift: Hold", 50, controlsY + 105, 14, GRAY);
    DrawText("R: Restart", 50, controlsY + 125, 14, GRAY);
}

void Renderer::drawGameOver() {
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // Semi-transparent overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});

    const char* text = "GAME OVER";
    int textWidth = MeasureText(text, 60);
    DrawText(text, centerX - textWidth / 2, centerY - 60, 60, RED);

    const char* restartText = "Press R to Restart";
    int restartWidth = MeasureText(restartText, 30);
    DrawText(restartText, centerX - restartWidth / 2, centerY + 20, 30, WHITE);
}
