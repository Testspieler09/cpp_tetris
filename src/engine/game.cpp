#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "engine/modes/igame_mode.hpp"
#include "engine/core/game.hpp"
#include "engine/core/piece_rotation.hpp"

Game::Game(std::unique_ptr<IGameMode> mode)
    : mode(std::move(mode)) {

    GameConfig cfg = this->mode->getInitialConfig();

    this->board_width = cfg.boardWidth;
    this->board_height = cfg.boardHeight;
    this->canHold = cfg.canHold;
    this->dropInterval = cfg.dropInterval;
    this->score = cfg.startingScore;
    this->level = cfg.startingLevel;
    this->linesCleared = cfg.startingLinesCleared;

    // Resize board
    this->board.resize(board_height);
    for (int y = 0; y < board_height; ++y) {
        this->board[y].resize(board_width);

        for (int x = 0; x < board_width; ++x) {
            this->board[y][x] = cfg.initialBoard[y][x];
        }
    }

    this->spawnNextPiece();
}

void Game::reset() {
    for (std::vector<char>& inner_vec : this->board) {
        std::fill(inner_vec.begin(), inner_vec.end(), 0);
    }
    this->mode->getInitialConfig();
    this->spawnNextPiece();
}

void Game::update(float deltaTime) {
    if (this->won || this->gameOver) {
        return;
    }

    this->dropTimer += deltaTime;

    if (this->dropTimer >= this->dropInterval) {
        this->dropTimer = 0.0f;

        if (!this->tryMoveDown()) {
            this->lockPiece();
            int cleared = this->clearLines();

            if (cleared > 0) {
                // Update score based on lines cleared
                int points[] = {0, 40, 100, 300, 1200};
                this->score += points[cleared] * this->level;
                this->linesCleared += cleared;

                // Level up every 10 lines
                this->level = (this->linesCleared / 10) + 1;
                this->updateDropInterval();
            }

            this->spawnNextPiece();
            this->canHold = true;

            if (this->mode->checkLose(*this)) {
                this->gameOver = true;
            }

            if (this->mode->checkWin(*this)) {
                this->won = true;
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
            this->tryMoveLeft();
            break;
        case GameEvent::MOVE_RIGHT:
            this->tryMoveRight();
            break;
        case GameEvent::MOVE_DOWN:
            if (this->tryMoveDown()) {
                this->score += 1; // Soft drop bonus
            }
            break;
        case GameEvent::ROTATE_CW:
            this->tryRotate(true);
            break;
        case GameEvent::ROTATE_CCW:
            this->tryRotate(false);
            break;
        case GameEvent::HARD_DROP:
            this->performHardDrop();
            break;
        case GameEvent::HOLD:
            this->performHold();
            break;
        case GameEvent::RESTART:
            this->reset();
            break;
    }
}

GameState Game::getState() const {
    GameState state;

    // Copy board
    state.board = this->board;

    // Current piece info
    this->currentPiece.getShape(state.currentPieceShape);
    state.currentPieceType = this->currentPiece.getType();
    state.currentPieceOrientation = this->currentPiece.getOrientation();
    state.currentPieceX = this->currentPiece.getX();
    state.currentPieceY = this->currentPiece.getY();

    // Hold piece info
    state.hasHeldPiece = this->heldPiece.has_value();
    state.canHold = this->canHold;
    state.heldPieceType = this->heldPiece ? this->heldPiece->getType() : TetrominoType::NONE;

    // Next pieces
    state.nextPieces = this->mode->getPiecePreview();

    // Ghost piece
    state.ghostPieceY = this->calculateGhostY();

    // Game stats
    state.score = this->score;
    state.level = this->level;
    state.linesCleared = this->linesCleared;
    state.gameOver = this->gameOver;

    return state;
}

bool Game::isValidPosition(const Tetromino& piece) const {
    return this->isValidPosition(piece, 0, 0);
}

bool Game::isValidPosition(const Tetromino& piece, int offsetX, int offsetY) const {
    char shape[4][4];
    piece.getShape(shape);

    int pieceX = piece.getX() + offsetX;
    int pieceY = piece.getY() + offsetY;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] != 0) {
                int boardX = pieceX + col;
                int boardY = pieceY + row;

                // Check horizontal bounds and bottom bound
                if (boardX < 0 || boardX >= board_width ||
                    boardY >= board_height) {
                    return false;
                }

                // Allow negative Y (piece above board)
                if (boardY >= 0) {
                    if (board[boardY][boardX] != 0) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void Game::lockPiece() {
    char shape[4][4];
    this->currentPiece.getShape(shape);

    int pieceX = this->currentPiece.getX();
    int pieceY = this->currentPiece.getY();
    int pieceType = static_cast<int>(this->currentPiece.getType());

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] != 0) {
                int boardX = pieceX + col;
                int boardY = pieceY + row;

                if (boardX >= 0 && boardX < this->board_width &&
                    boardY >= 0 && boardY < this->board_height) {
                    this->board[boardY][boardX] = pieceType;
                }
            }
        }
    }
}

int Game::clearLines() {
    int cleared = 0;

    for (int row = this->board_height - 1; row >= 0; row--) {
        bool fullLine = true;

        for (int col = 0; col < this->board_width; col++) {
            if (this->board[row][col] == 0) {
                fullLine = false;
                break;
            }
        }

        if (fullLine) {
            cleared++;

            // Move all rows above down
            for (int r = row; r > 0; r--) {
                for (int col = 0; col < this->board_width; col++) {
                    this->board[r][col] = this->board[r - 1][col];
                }
            }

            // Clear top row
            for (int col = 0; col < this->board_width; col++) {
                this->board[0][col] = 0;
            }

            // Check this row again
            row++;
        }
    }

    return cleared;
}

void Game::spawnNextPiece() {
    this->currentPiece = this->mode->getNextPiece(this->SPAWN_X, this->SPAWN_Y);
}

int Game::calculateGhostY() const {
    Tetromino ghost = this->currentPiece;

    while (this->isValidPosition(ghost, 0, 1)) {
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
    if (this->isValidPosition(this->currentPiece, 0, 1)) {
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

        if (this->isValidPosition(testPiece)) {
            this->currentPiece = testPiece;
            return true;
        }
    }

    return false;
}

void Game::performHardDrop() {
    int ghostY = this->calculateGhostY();
    int distance = ghostY - this->currentPiece.getY();

    this->currentPiece.setPosition(this->currentPiece.getX(), ghostY);
    this->score += distance * 2; // Hard drop bonus

    this->lockPiece();
    int cleared = this->clearLines();

    if (cleared > 0) {
        int points[] = {0, 40, 100, 300, 1200};
        this->score += points[cleared] * this->level;
        this->linesCleared += cleared;
        this->level = (this->linesCleared / 10) + 1;
        this->updateDropInterval();
    }

    this->spawnNextPiece();
    this->canHold = true;

    if (this->mode->checkLose(*this)) {
        this->gameOver = true;
    }

    if (this->mode->checkWin(*this)) {
        this->won = true;
    }

    this->dropTimer = 0.0f;
}

void Game::performHold() {
    if (!this->canHold) {
        return;
    }

    this->canHold = false;

    if (this->heldPiece) {
        // Swap current piece with held piece
        Tetromino temp = this->currentPiece;
        this->currentPiece = Tetromino(this->heldPiece->getType(), this->SPAWN_X, this->SPAWN_Y);
        this->heldPiece = Tetromino(temp.getType(), 0, 0);
    } else {
        // Store current piece and spawn new one
        this->heldPiece = Tetromino(this->currentPiece.getType(), 0, 0);
        this->spawnNextPiece();
    }
}
