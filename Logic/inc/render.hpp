#pragma once
#include "board.hpp"
#include "gamestate.hpp"
#include "position.hpp"
#include "player.hpp"

namespace Chess {

class Render {
public:
    virtual ~Render() = default;
    virtual void show_menu() = 0;
    virtual void show_board(const Logic::Board &board) = 0;
    virtual void show_next_step(Logic::Player *player) = 0;
    virtual void show_error(Logic::GameState state) = 0;
    virtual void show_pawn_promotion(const Chess::Logic::Board& board, const Logic::Position &pos) = 0;
    virtual void show_endgame(Logic::GameState state, Logic::Player *winer) = 0;
};

} // namespace Chess
