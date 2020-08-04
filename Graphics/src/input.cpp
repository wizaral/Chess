#include "g_input.hpp"
#include <iostream>

Chess::Logic::Move GraphicsInput::get_next_move(Chess::Logic::Player *player) {
    return player->get_next_move();
}

Chess::Logic::FigureType GraphicsInput::promote_figure(Chess::Logic::Player *player, const Chess::Logic::Position &pos) {
    return player->promote_figure(pos);
}
