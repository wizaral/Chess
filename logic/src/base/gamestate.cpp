#include "gamestate.hpp"

namespace Chess {

bool is_move(GameState gstate) {
    return gstate == GameState::NormalMove
        || gstate == GameState::EnPassant
        || gstate == GameState::KingCastling
        || gstate == GameState::QueenCastling
        || gstate == GameState::PawnPromotion;
}

bool is_endgame(GameState gstate) {
    return gstate == GameState::CheckMate
        || gstate == GameState::StaleMate
        || gstate == GameState::Draw;
}

bool is_error(GameState gstate) {
    return !is_move(gstate) && !is_endgame(gstate);
}

} // namespace Chess