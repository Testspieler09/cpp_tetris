#pragma once

#include <vector>

struct GameConfig {
    int boardWidth = 10;
    int boardHeight = 20;
    bool canHoldAnything = true;
    float dropInterval = 1.0f;
    int startingScore = 0;
    int startingLevel = 1;
    int startingLinesCleared = 0;
    std::vector<std::vector<char>> initialBoard;

    GameConfig()
        : initialBoard(this->boardHeight, std::vector<char>(this->boardWidth, 0))
    {}
};
