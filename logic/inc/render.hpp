#pragma once
#include "board.hpp"

namespace Chess {

class Render {
public:
    virtual void show_menu() = 0;
    virtual void show_board(const Logic::Board &board) = 0;
    virtual void show_error(Logic::GameState state) = 0;
    virtual void show_endgame(Logic::GameState state, Logic::Player *winer) = 0;
};

} // namespace Chess
