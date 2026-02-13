#pragma once

struct GameConfig {
    int boardWidth = 10;
    int boardHeight = 20;
    bool canHold = true;
    float dropInterval = 1.0f;
    int startingScore = 0;
    int startingLevel = 1;
    int startingLinesCleared = 0;

    char initialBoard[20][10]{};
};
