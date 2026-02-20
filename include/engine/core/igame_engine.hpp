#pragma once

#include <array>
#include <vector>

enum class TetrominoType {
    NONE = 0,
    I = 1,
    O = 2,
    T = 3,
    S = 4,
    Z = 5,
    J = 6,
    L = 7,
    BLOCK = 8
};

enum class Orientation {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

enum class GameEvent {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE_CW,
    ROTATE_CCW,
    HARD_DROP,
    HOLD,
    RESTART
};

struct GameState {
    // Board state
    std::vector<std::vector<char>> board;

    // Current piece
    char currentPieceShape[4][4];
    TetrominoType currentPieceType;
    Orientation currentPieceOrientation;
    int currentPieceX;
    int currentPieceY;

    // Hold piece
    bool hasHeldPiece;
    bool canHold;
    TetrominoType heldPieceType;

    // Next pieces preview (2 pieces)
    std::array<TetrominoType, 2> nextPieces;

    // Ghost piece (shows where piece will land)
    int ghostPieceY;

    // Game stats
    int score;
    int level;
    int linesCleared;
    bool gameOver;
    bool won;
};

class IGameEngine {
public:
    virtual ~IGameEngine() = default;

    virtual void update(float deltaTime) = 0;
    virtual void handleEvent(GameEvent event) = 0;
    virtual GameState getState() const = 0;
};
