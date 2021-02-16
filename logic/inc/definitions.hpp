#pragma once
#include <array>

namespace Chess {

inline static constexpr int board_rows = 8;
inline static constexpr int board_cols = 8;

inline static constexpr std::array<int, 2> pawns_row{
    1,
    6,
};

inline static constexpr std::array<int, 2> figures_row{
    0,
    7,
};

inline static constexpr std::array<int, 2> step_direction{
    1,
    -1,
};

} // namespace Chess
