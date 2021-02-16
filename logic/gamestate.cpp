#include "gamestate.hpp"

namespace Chess {

bool is_move(GameState state) {
    return static_cast<int>(state) < 5;
}

bool is_castling(GameState state) {
    return state == GameState::KingCastling || state == GameState::QueenCastling;
}

bool is_endgame(GameState state) {
    return static_cast<int>(state) > 4 && static_cast<int>(state) < 8;
}

bool is_error(GameState state) {
    return static_cast<int>(state) > 7;
}

} // namespace Chess
