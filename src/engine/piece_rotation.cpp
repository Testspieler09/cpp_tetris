#include "engine/core/piece_rotation.hpp"

std::vector<std::pair<int, int>> PieceRotation::getWallKicks(
    TetrominoType type,
    Orientation fromOrientation,
    Orientation toOrientation
) {
    std::vector<std::pair<int, int>> kicks;

    if (type == TetrominoType::O) {
        kicks.push_back({0, 0});
        return kicks;
    }

    int fromIndex = static_cast<int>(fromOrientation);
    int toIndex   = static_cast<int>(toOrientation);

    const int (*table)[4][5][2] =
        (type == TetrominoType::I) ? I_KICKS : JLSTZ_KICKS;

    for (int i = 0; i < 5; i++) {
        kicks.emplace_back(
            table[fromIndex][toIndex][i][0],
            table[fromIndex][toIndex][i][1]
        );
    }

    return kicks;
}

Orientation PieceRotation::getNextOrientation(Orientation current, bool clockwise) {
    int index = static_cast<int>(current);

    if (clockwise) {
        index = (index + 1) % 4;
    } else {
        index = (index - 1 + 4) % 4;
    }

    return static_cast<Orientation>(index);
}
