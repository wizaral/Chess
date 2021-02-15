#include "gamestate.hpp"

namespace Chess {

bool is_move(GameState state) {
    return state == GameState::NormalMove
        || state == GameState::EnPassant
        || state == GameState::KingCastling
        || state == GameState::QueenCastling
        || state == GameState::PawnPromotion;
}

bool is_endgame(GameState state) {
    return state == GameState::CheckMate
        || state == GameState::StaleMate
        || state == GameState::Draw;
}

bool is_error(GameState state) {
    return !is_move(state) && !is_endgame(state);
}

} // namespace Chess
