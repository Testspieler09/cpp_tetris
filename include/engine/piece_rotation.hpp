#pragma once

#include "igame_engine.hpp"
#include <utility>
#include <vector>

class PieceRotation {
public:
    // SRS wall kick data
    // For JLSTZ pieces: 5 kick tests per rotation
    static constexpr int JLSTZ_KICKS[4][4][5][2] = {
        // 0->1 (N->E)
        {
            {{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}},  // Test 0->1
            {{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},      // Test 1->0
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}}        // Unused
        },
        // 1->2 (E->S)
        {
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},     // Test 1->2
            {{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}},   // Test 2->1
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}}        // Unused
        },
        // 2->3 (S->W)
        {
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},      // Test 2->3
            {{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}}   // Test 3->2
        },
        // 3->0 (W->N)
        {
            {{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}},   // Test 3->0
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}}      // Test 0->3
        }
    };

    // I-piece has different wall kicks
    static constexpr int I_KICKS[4][4][5][2] = {
        // 0->1 (N->E)
        {
            {{0,0}, {-2,0}, {1,0}, {-2,-1}, {1,2}},    // Test 0->1
            {{0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2}},    // Test 1->0
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}}        // Unused
        },
        // 1->2 (E->S)
        {
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1}},    // Test 1->2
            {{0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1}},    // Test 2->1
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}}        // Unused
        },
        // 2->3 (S->W)
        {
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {2,0}, {-1,0}, {2,1}, {-1,-2}},    // Test 2->3
            {{0,0}, {-2,0}, {1,0}, {-2,-1}, {1,2}}     // Test 3->2
        },
        // 3->0 (W->N)
        {
            {{0,0}, {1,0}, {-2,0}, {1,-2}, {-2,1}},    // Test 3->0
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}},       // Unused
            {{0,0}, {-1,0}, {2,0}, {-1,2}, {2,-1}}     // Test 0->3
        }
    };

    // O-piece doesn't rotate, but included for completeness
    static constexpr int O_KICKS[4][4][1][2] = {
        {{{0,0}}, {{0,0}}, {{0,0}}, {{0,0}}},
        {{{0,0}}, {{0,0}}, {{0,0}}, {{0,0}}},
        {{{0,0}}, {{0,0}}, {{0,0}}, {{0,0}}},
        {{{0,0}}, {{0,0}}, {{0,0}}, {{0,0}}}
    };

    // Get wall kick offsets for a rotation
    // Returns vector of (dx, dy) pairs to try
    static std::vector<std::pair<int, int>> getWallKicks(
        TetrominoType type,
        Orientation fromOrientation,
        Orientation toOrientation
    );

    // Get the next orientation when rotating
    static Orientation getNextOrientation(Orientation current, bool clockwise);
};
