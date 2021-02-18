#pragma once
#include <array>

namespace Chess {

inline static constexpr int board_rows = 8;
inline static constexpr int board_cols = 8;

inline static constexpr int players_amount = 2;
inline static constexpr int figure_types_amount = 6;

inline static constexpr std::array<int, players_amount> pawns_row{
    1,
    6,
};

inline static constexpr std::array<int, players_amount> figures_row{
    0,
    7,
};

inline static constexpr std::array<int, players_amount> step_direction{
    1,
    -1,
};

} // namespace Chess
