#include "c_input.hpp"

Chess::Logic::Move ConsoleInput::get_next_move(Chess::Logic::Player *player) {
    return {{0, 1}, {0, 0}};
}

Chess::Logic::FigureType ConsoleInput::promote_figure(Chess::Logic::Player *player, Chess::Logic::Position pos) {
    return Chess::Logic::FigureType::Pawn;
}

Chess::Logic::Position ConsoleInput::transform(int row, char col) {
    return {0, 1};
}
