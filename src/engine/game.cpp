#include "engine/game.hpp"
#include "engine/piece_rotation.hpp"
#include <cstring>
#include <algorithm>

Game::Game()
    : canHold(true), score(0), level(1), linesCleared(0),
      gameOver(false), dropTimer(0.0f), dropInterval(1.0f) {
    // Initialize board
    std::memset(board, 0, sizeof(board));

    // Spawn first piece
    spawnNextPiece();
}

void Game::reset() {
    std::memset(board, 0, sizeof(board));
    score = 0;
    level = 1;
    linesCleared = 0;
    gameOver = false;
    dropTimer = 0.0f;
    dropInterval = 1.0f;
    canHold = true;
    heldPiece.reset();

    // Reset generator and spawn new piece
    generator = PieceGenerator();
    spawnNextPiece();
}

void Game::update(float deltaTime) {
    if (gameOver) {
        return;
    }

    dropTimer += deltaTime;

    if (dropTimer >= dropInterval) {
        dropTimer = 0.0f;

        if (!tryMoveDown()) {
            lockPiece();
            int cleared = clearLines();

            if (cleared > 0) {
                // Update score based on lines cleared
                int points[] = {0, 40, 100, 300, 1200};
                score += points[cleared] * level;
                linesCleared += cleared;

                // Level up every 10 lines
                level = (linesCleared / 10) + 1;
                updateDropInterval();
            }

            spawnNextPiece();
            canHold = true;

            // Check for game over
            if (!isValidPosition(currentPiece)) {
                gameOver = true;
            }
        }
    }
}

void Game::handleEvent(GameEvent event) {
    if (gameOver && event != GameEvent::RESTART) {
        return;
    }

    switch (event) {
        case GameEvent::MOVE_LEFT:
            tryMoveLeft();
            break;
        case GameEvent::MOVE_RIGHT:
            tryMoveRight();
            break;
        case GameEvent::MOVE_DOWN:
            if (tryMoveDown()) {
                score += 1; // Soft drop bonus
            }
            break;
        case GameEvent::ROTATE_CW:
            tryRotate(true);
            break;
        case GameEvent::ROTATE_CCW:
            tryRotate(false);
            break;
        case GameEvent::HARD_DROP:
            performHardDrop();
            break;
        case GameEvent::HOLD:
            performHold();
            break;
        case GameEvent::RESTART:
            reset();
            break;
    }
}

GameState Game::getState() const {
    GameState state;

    // Copy board
    std::memcpy(state.board, board, sizeof(board));

    // Current piece info
    currentPiece.getShape(state.currentPieceShape);
    state.currentPieceType = currentPiece.getType();
    state.currentPieceOrientation = currentPiece.getOrientation();
    state.currentPieceX = currentPiece.getX();
    state.currentPieceY = currentPiece.getY();

    // Hold piece info
    state.hasHeldPiece = heldPiece.has_value();
    state.canHold = canHold;
    state.heldPieceType = heldPiece.has_value() ? heldPiece->getType() : TetrominoType::NONE;

    // Next pieces
    state.nextPieces = generator.getPreview();

    // Ghost piece
    state.ghostPieceY = calculateGhostY();

    // Game stats
    state.score = score;
    state.level = level;
    state.linesCleared = linesCleared;
    state.gameOver = gameOver;

    return state;
}

bool Game::isValidPosition(const Tetromino& piece) const {
    return isValidPosition(piece, 0, 0);
}

bool Game::isValidPosition(const Tetromino& piece, int offsetX, int offsetY) const {
    int shape[4][4];
    piece.getShape(shape);

    int pieceX = piece.getX() + offsetX;
    int pieceY = piece.getY() + offsetY;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] != 0) {
                int boardX = pieceX + col;
                int boardY = pieceY + row;

                // Check bounds
                if (boardX < 0 || boardX >= BOARD_WIDTH ||
                    boardY < 0 || boardY >= BOARD_HEIGHT) {
                    return false;
                }

                // Check collision with locked pieces
                if (board[boardY][boardX] != 0) {
                    return false;
                }
            }
        }
    }

    return true;
}

void Game::lockPiece() {
    int shape[4][4];
    currentPiece.getShape(shape);

    int pieceX = currentPiece.getX();
    int pieceY = currentPiece.getY();
    int pieceType = static_cast<int>(currentPiece.getType());

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] != 0) {
                int boardX = pieceX + col;
                int boardY = pieceY + row;

                if (boardX >= 0 && boardX < BOARD_WIDTH &&
                    boardY >= 0 && boardY < BOARD_HEIGHT) {
                    board[boardY][boardX] = pieceType;
                }
            }
        }
    }
}

int Game::clearLines() {
    int cleared = 0;

    for (int row = BOARD_HEIGHT - 1; row >= 0; row--) {
        bool fullLine = true;

        for (int col = 0; col < BOARD_WIDTH; col++) {
            if (board[row][col] == 0) {
                fullLine = false;
                break;
            }
        }

        if (fullLine) {
            cleared++;

            // Move all rows above down
            for (int r = row; r > 0; r--) {
                for (int col = 0; col < BOARD_WIDTH; col++) {
                    board[r][col] = board[r - 1][col];
                }
            }

            // Clear top row
            for (int col = 0; col < BOARD_WIDTH; col++) {
                board[0][col] = 0;
            }

            // Check this row again
            row++;
        }
    }

    return cleared;
}

void Game::spawnNextPiece() {
    currentPiece = generator.getNext();
}

int Game::calculateGhostY() const {
    Tetromino ghost = currentPiece;

    while (isValidPosition(ghost, 0, 1)) {
        ghost.moveDown();
    }

    return ghost.getY();
}

void Game::updateDropInterval() {
    // Decrease drop interval as level increases
    dropInterval = std::max(0.1f, 1.0f - (level - 1) * 0.05f);
}

bool Game::tryMoveLeft() {
    if (isValidPosition(currentPiece, -1, 0)) {
        currentPiece.moveLeft();
        return true;
    }
    return false;
}

bool Game::tryMoveRight() {
    if (isValidPosition(currentPiece, 1, 0)) {
        currentPiece.moveRight();
        return true;
    }
    return false;
}

bool Game::tryMoveDown() {
    if (isValidPosition(currentPiece, 0, 1)) {
        currentPiece.moveDown();
        return true;
    }
    return false;
}

bool Game::tryRotate(bool clockwise) {
    Orientation currentOri = currentPiece.getOrientation();
    Orientation newOri = PieceRotation::getNextOrientation(currentOri, clockwise);

    // Get wall kicks for this rotation
    auto kicks = PieceRotation::getWallKicks(
        currentPiece.getType(),
        currentOri,
        newOri
    );

    // Try each kick offset
    for (const auto& kick : kicks) {
        Tetromino testPiece = currentPiece;
        testPiece.setOrientation(newOri);
        testPiece.setPosition(
            currentPiece.getX() + kick.first,
            currentPiece.getY() + kick.second
        );

        if (isValidPosition(testPiece)) {
            currentPiece = testPiece;
            return true;
        }
    }

    return false;
}

void Game::performHardDrop() {
    int ghostY = calculateGhostY();
    int distance = ghostY - currentPiece.getY();

    currentPiece.setPosition(currentPiece.getX(), ghostY);
    score += distance * 2; // Hard drop bonus

    lockPiece();
    int cleared = clearLines();

    if (cleared > 0) {
        int points[] = {0, 40, 100, 300, 1200};
        score += points[cleared] * level;
        linesCleared += cleared;
        level = (linesCleared / 10) + 1;
        updateDropInterval();
    }

    spawnNextPiece();
    canHold = true;

    if (!isValidPosition(currentPiece)) {
        gameOver = true;
    }

    dropTimer = 0.0f;
}

void Game::performHold() {
    if (!canHold) {
        return;
    }

    canHold = false;

    if (heldPiece.has_value()) {
        // Swap current piece with held piece
        Tetromino temp = currentPiece;
        currentPiece = Tetromino(heldPiece->getType(), SPAWN_X, SPAWN_Y);
        heldPiece = Tetromino(temp.getType(), 0, 0);
    } else {
        // Store current piece and spawn new one
        heldPiece = Tetromino(currentPiece.getType(), 0, 0);
        spawnNextPiece();
    }
}
