#include "ui/renderer.hpp"
#include "engine/tetromino.hpp"
#include "raylib.h"
#include <cstring>

Renderer::Renderer(IGameEngine& game, int width, int height, int cellSize)
    : gameEngine(game), screenWidth(width), screenHeight(height),
      cellSize(cellSize), tickAccumulator(0.0f) {

    // Calculate layout
    this->boardOffsetX = 250;
    this->boardOffsetY = 50;

    this->holdBoxX = 50;
    this->holdBoxY = 50;

    this->nextBoxX = this->boardOffsetX + (10 * this->cellSize) + 50;
    this->nextBoxY = 50;

    // Initialize window
    InitWindow(this->screenWidth, this->screenHeight, "Tetris");
    SetTargetFPS(60);

    setupDefaultKeyMapping();
}

Renderer::~Renderer() {
    CloseWindow();
}

void Renderer::setupDefaultKeyMapping() {
    mapKey(KEY_LEFT, GameEvent::MOVE_LEFT);
    mapKey(KEY_H, GameEvent::MOVE_LEFT);

    mapKey(KEY_RIGHT, GameEvent::MOVE_RIGHT);
    mapKey(KEY_L, GameEvent::MOVE_RIGHT);

    mapKey(KEY_DOWN, GameEvent::MOVE_DOWN);
    mapKey(KEY_J, GameEvent::MOVE_DOWN);

    mapKey(KEY_UP, GameEvent::HARD_DROP);
    mapKey(KEY_K, GameEvent::HARD_DROP);

    mapKey(KEY_Z, GameEvent::ROTATE_CCW);
    mapKey(KEY_X, GameEvent::ROTATE_CW);

    mapKey(KEY_SPACE, GameEvent::HOLD);

    mapKey(KEY_R, GameEvent::RESTART);
}

void Renderer::mapKey(int raylibKey, GameEvent event) {
    this->keyMapping[raylibKey] = event;
}

void Renderer::clearKeyMapping() {
    this->keyMapping.clear();
}

void Renderer::run() {
    while (!WindowShouldClose()) {
        float frameTime = GetFrameTime();

        processInput();

        // Fixed timestep update (60 ticks per second)
        tickAccumulator += frameTime;
        while (tickAccumulator >= TARGET_TICK_RATE) {
            this->gameEngine.update(TARGET_TICK_RATE);
            tickAccumulator -= TARGET_TICK_RATE;
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        GameState state = this->gameEngine.getState();

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
    for (const std::pair<const int, GameEvent>& mapping : this->keyMapping) {
        if (IsKeyPressed(mapping.first) || IsKeyPressedRepeat(mapping.first)) {
            this->gameEngine.handleEvent(mapping.second);
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

    int x = this->boardOffsetX + gridX * this->cellSize;
    int y = this->boardOffsetY + gridY * this->cellSize;

    DrawRectangle(x + 1, y + 1, this->cellSize - 2, this->cellSize - 2, color);
    DrawRectangleLines(x, y, this->cellSize, this->cellSize, WHITE);
}

void Renderer::drawPieceShape(const int shape[4][4], int offsetX, int offsetY,
                               TetrominoType type, float alpha) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] != 0) {
                Color color = getColorForType(type);
                color.a = static_cast<unsigned char>(255 * alpha);

                int x = offsetX + col * this->cellSize;
                int y = offsetY + row * this->cellSize;

                DrawRectangle(x + 1, y + 1, this->cellSize - 2, this->cellSize - 2, color);
                DrawRectangleLines(x, y, this->cellSize, this->cellSize, WHITE);
            }
        }
    }
}

void Renderer::drawBoard(const GameState& state) {
    // Draw board background
    DrawRectangle(this->boardOffsetX, this->boardOffsetY,
                  10 * this->cellSize, 20 * this->cellSize,
                  {20, 20, 20, 255});

    // Draw grid and locked pieces
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
            int x = this->boardOffsetX + col * this->cellSize;
            int y = this->boardOffsetY + row * this->cellSize;

            DrawRectangleLines(x, y, this->cellSize, this->cellSize, {50, 50, 50, 255});

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
    DrawText("HOLD", this->holdBoxX, this->holdBoxY - 25, 20, WHITE);

    // Draw hold box background
    DrawRectangle(this->holdBoxX, this->holdBoxY, 4 * this->cellSize, 4 * this->cellSize, {20, 20, 20, 255});
    DrawRectangleLines(this->holdBoxX, this->holdBoxY, 4 * this->cellSize, 4 * this->cellSize, WHITE);

    if (state.hasHeldPiece) {
        int heldShape[4][4];
        Tetromino::getBaseShape(state.heldPieceType, Orientation::NORTH, heldShape);

        float alpha = state.canHold ? 1.0f : 0.4f;
        drawPieceShape(heldShape, this->holdBoxX, this->holdBoxY, state.heldPieceType, alpha);
    }
}

void Renderer::drawNextBox(const GameState& state) {
    DrawText("NEXT", this->nextBoxX, this->nextBoxY - 25, 20, WHITE);

    int yOffset = this->nextBoxY;

    for (int i = 0; i < 2; i++) {
        // Draw next box background
        DrawRectangle(this->nextBoxX, yOffset, 4 * this->cellSize, 4 * this->cellSize, {20, 20, 20, 255});
        DrawRectangleLines(this->nextBoxX, yOffset, 4 * this->cellSize, 4 * this->cellSize, WHITE);

        if (state.nextPieces[i] != TetrominoType::NONE) {
            int nextShape[4][4];
            Tetromino::getBaseShape(state.nextPieces[i], Orientation::NORTH, nextShape);
            drawPieceShape(nextShape, this->nextBoxX, yOffset, state.nextPieces[i], 1.0f);
        }

        yOffset += 4 * this->cellSize + 20;
    }
}

void Renderer::drawUI(const GameState& state) {
    int uiX = this->nextBoxX;
    int uiY = this->nextBoxY + 2 * (4 * this->cellSize + 20) + 30;

    DrawText(TextFormat("SCORE: %d", state.score), uiX, uiY, 20, WHITE);
    DrawText(TextFormat("LEVEL: %d", state.level), uiX, uiY + 30, 20, WHITE);
    DrawText(TextFormat("LINES: %d", state.linesCleared), uiX, uiY + 60, 20, WHITE);

    // Controls
    int controlsY = this->screenHeight - 200;
    DrawText("CONTROLS:", 50, controlsY, 16, GRAY);
    DrawText("Arrows: Move", 50, controlsY + 25, 14, GRAY);
    DrawText("X/Z: Rotate", 50, controlsY + 45, 14, GRAY);
    DrawText("Arrow Up: Hard Drop", 50, controlsY + 65, 14, GRAY);
    DrawText("Space: Hold", 50, controlsY + 85, 14, GRAY);
    DrawText("R: Restart", 50, controlsY + 105, 14, GRAY);
}

void Renderer::drawGameOver() {
    int centerX = this->screenWidth / 2;
    int centerY = this->screenHeight / 2;

    // Semi-transparent overlay
    DrawRectangle(0, 0, this->screenWidth, this->screenHeight, {0, 0, 0, 180});

    const char* text = "GAME OVER";
    int textWidth = MeasureText(text, 60);
    DrawText(text, centerX - textWidth / 2, centerY - 60, 60, RED);

    const char* restartText = "Press R to Restart";
    int restartWidth = MeasureText(restartText, 30);
    DrawText(restartText, centerX - restartWidth / 2, centerY + 20, 30, WHITE);
}
