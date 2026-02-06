#include "engine/piece_rotation.hpp"

std::vector<std::pair<int, int>> PieceRotation::getWallKicks(
    TetrominoType type,
    Orientation fromOrientation,
    Orientation toOrientation
) {
    std::vector<std::pair<int, int>> kicks;

    // O piece doesn't rotate
    if (type == TetrominoType::O) {
        kicks.push_back({0, 0});
        return kicks;
    }

    int fromIndex = static_cast<int>(fromOrientation);
    int toIndex = static_cast<int>(toOrientation);

    // Determine which table index to use based on rotation direction
    int tableIndex = fromIndex;
    int kickIndex;

    // Figure out rotation direction and corresponding kick index
    if ((fromIndex == 0 && toIndex == 1) || (fromIndex == 3 && toIndex == 0)) {
        kickIndex = 0; // Clockwise from current state
    } else if ((fromIndex == 1 && toIndex == 0) || (fromIndex == 0 && toIndex == 3)) {
        kickIndex = 1; // Counter-clockwise from current state
    } else if ((fromIndex == 1 && toIndex == 2) || (fromIndex == 2 && toIndex == 1)) {
        kickIndex = (fromIndex == 1) ? 1 : 2;
        tableIndex = 1;
    } else if ((fromIndex == 2 && toIndex == 3) || (fromIndex == 3 && toIndex == 2)) {
        kickIndex = (fromIndex == 2) ? 2 : 3;
        tableIndex = 2;
    } else {
        // Shouldn't happen with valid rotations
        kicks.push_back({0, 0});
        return kicks;
    }

    // Select appropriate kick table
    if (type == TetrominoType::I) {
        for (int i = 0; i < 5; i++) {
            kicks.push_back({
                I_KICKS[tableIndex][kickIndex][i][0],
                I_KICKS[tableIndex][kickIndex][i][1]
            });
        }
    } else {
        // J, L, S, T, Z all use the same table
        for (int i = 0; i < 5; i++) {
            kicks.push_back({
                JLSTZ_KICKS[tableIndex][kickIndex][i][0],
                JLSTZ_KICKS[tableIndex][kickIndex][i][1]
            });
        }
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
