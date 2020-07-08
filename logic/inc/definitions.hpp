#pragma once

namespace Chess::Logic {

inline constexpr int board_rows = 8;
inline constexpr int board_cols = 8;

inline constexpr int player0_pawns_row = 1;
inline constexpr int player1_pawns_row = 6;

inline constexpr int player0_figures_row = 0;
inline constexpr int player1_figures_row = 7;

inline constexpr int player0_step_direction = -1;
inline constexpr int player1_step_direction = 1;

} // namespace Chess::Logic
