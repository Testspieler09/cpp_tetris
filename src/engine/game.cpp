#include "engine/game.hpp"
#include "engine/piece_rotation.hpp"
#include <cstring>
#include <algorithm>

Game::Game()
    : canHold(true), score(0), level(1), linesCleared(0),
      gameOver(false), dropTimer(0.0f), dropInterval(1.0f) {
    std::memset(this->board, 0, sizeof(this->board));
    spawnNextPiece();
}

void Game::reset() {
    std::memset(this->board, 0, sizeof(this->board));
    this->score = 0;
    this->level = 1;
    this->linesCleared = 0;
    this->gameOver = false;
    this->dropTimer = 0.0f;
    this->dropInterval = 1.0f;
    this->canHold = true;
    this->heldPiece.reset();

    this->generator = PieceGenerator();
    spawnNextPiece();
}

void Game::update(float deltaTime) {
    if (this->gameOver) {
        return;
    }

    this->dropTimer += deltaTime;

    if (this->dropTimer >= this->dropInterval) {
        this->dropTimer = 0.0f;

        if (!tryMoveDown()) {
            lockPiece();
            int cleared = clearLines();

            if (cleared > 0) {
                // Update score based on lines cleared
                int points[] = {0, 40, 100, 300, 1200};
                this->score += points[cleared] * this->level;
                this->linesCleared += cleared;

                // Level up every 10 lines
                this->level = (this->linesCleared / 10) + 1;
                updateDropInterval();
            }

            spawnNextPiece();
            this->canHold = true;

            if (!isValidPosition(this->currentPiece)) {
                this->gameOver = true;
            }
        }
    }
}

void Game::handleEvent(GameEvent event) {
    if (this->gameOver && event != GameEvent::RESTART) {
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
                this->score += 1; // Soft drop bonus
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
    std::memcpy(state.board, this->board, sizeof(this->board));

    // Current piece info
    this->currentPiece.getShape(state.currentPieceShape);
    state.currentPieceType = this->currentPiece.getType();
    state.currentPieceOrientation = this->currentPiece.getOrientation();
    state.currentPieceX = this->currentPiece.getX();
    state.currentPieceY = this->currentPiece.getY();

    // Hold piece info
    state.hasHeldPiece = this->heldPiece.has_value();
    state.canHold = this->canHold;
    state.heldPieceType = this->heldPiece.has_value() ? this->heldPiece->getType() : TetrominoType::NONE;

    // Next pieces
    state.nextPieces = this->generator.getPreview();

    // Ghost piece
    state.ghostPieceY = calculateGhostY();

    // Game stats
    state.score = this->score;
    state.level = this->level;
    state.linesCleared = this->linesCleared;
    state.gameOver = this->gameOver;

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
                if (this->board[boardY][boardX] != 0) {
                    return false;
                }
            }
        }
    }

    return true;
}

void Game::lockPiece() {
    int shape[4][4];
    this->currentPiece.getShape(shape);

    int pieceX = this->currentPiece.getX();
    int pieceY = this->currentPiece.getY();
    int pieceType = static_cast<int>(this->currentPiece.getType());

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] != 0) {
                int boardX = pieceX + col;
                int boardY = pieceY + row;

                if (boardX >= 0 && boardX < BOARD_WIDTH &&
                    boardY >= 0 && boardY < BOARD_HEIGHT) {
                    this->board[boardY][boardX] = pieceType;
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
            if (this->board[row][col] == 0) {
                fullLine = false;
                break;
            }
        }

        if (fullLine) {
            cleared++;

            // Move all rows above down
            for (int r = row; r > 0; r--) {
                for (int col = 0; col < BOARD_WIDTH; col++) {
                    this->board[r][col] = this->board[r - 1][col];
                }
            }

            // Clear top row
            for (int col = 0; col < BOARD_WIDTH; col++) {
                this->board[0][col] = 0;
            }

            // Check this row again
            row++;
        }
    }

    return cleared;
}

void Game::spawnNextPiece() {
    this->currentPiece = this->generator.getNext();
}

int Game::calculateGhostY() const {
    Tetromino ghost = this->currentPiece;

    while (isValidPosition(ghost, 0, 1)) {
        ghost.moveDown();
    }

    return ghost.getY();
}

void Game::updateDropInterval() {
    // Decrease drop interval as level increases
    this->dropInterval = std::max(0.1f, 1.0f - (this->level - 1) * 0.05f);
}

bool Game::tryMoveLeft() {
    if (isValidPosition(this->currentPiece, -1, 0)) {
        this->currentPiece.moveLeft();
        return true;
    }
    return false;
}

bool Game::tryMoveRight() {
    if (isValidPosition(this->currentPiece, 1, 0)) {
        this->currentPiece.moveRight();
        return true;
    }
    return false;
}

bool Game::tryMoveDown() {
    if (isValidPosition(this->currentPiece, 0, 1)) {
        this->currentPiece.moveDown();
        return true;
    }
    return false;
}

bool Game::tryRotate(bool clockwise) {
    Orientation currentOri = this->currentPiece.getOrientation();
    Orientation newOri = PieceRotation::getNextOrientation(currentOri, clockwise);

    // Get wall kicks for this rotation
    std::vector<std::pair<int, int>> kicks = PieceRotation::getWallKicks(
        this->currentPiece.getType(),
        currentOri,
        newOri
    );

    // Try each kick offset
    for (const std::pair<int, int>& kick : kicks) {
        Tetromino testPiece = this->currentPiece;
        testPiece.setOrientation(newOri);
        testPiece.setPosition(
            this->currentPiece.getX() + kick.first,
            this->currentPiece.getY() + kick.second
        );

        if (isValidPosition(testPiece)) {
            this->currentPiece = testPiece;
            return true;
        }
    }

    return false;
}

void Game::performHardDrop() {
    int ghostY = calculateGhostY();
    int distance = ghostY - this->currentPiece.getY();

    this->currentPiece.setPosition(this->currentPiece.getX(), ghostY);
    this->score += distance * 2; // Hard drop bonus

    lockPiece();
    int cleared = clearLines();

    if (cleared > 0) {
        int points[] = {0, 40, 100, 300, 1200};
        this->score += points[cleared] * this->level;
        this->linesCleared += cleared;
        this->level = (this->linesCleared / 10) + 1;
        updateDropInterval();
    }

    spawnNextPiece();
    this->canHold = true;

    if (!isValidPosition(this->currentPiece)) {
        this->gameOver = true;
    }

    this->dropTimer = 0.0f;
}

void Game::performHold() {
    if (!this->canHold) {
        return;
    }

    this->canHold = false;

    if (this->heldPiece.has_value()) {
        // Swap current piece with held piece
        Tetromino temp = this->currentPiece;
        this->currentPiece = Tetromino(this->heldPiece->getType(), SPAWN_X, SPAWN_Y);
        this->heldPiece = Tetromino(temp.getType(), 0, 0);
    } else {
        // Store current piece and spawn new one
        this->heldPiece = Tetromino(this->currentPiece.getType(), 0, 0);
        spawnNextPiece();
    }
}
