#include <iostream>

#include "t_input.hpp"

Chess::Logic::Move TestInput::get_next_move(Chess::Logic::Player *player) {
    std::string input;
    std::cin >> input;

    char col_from = input[0], col_to = input[2];
    int row_from = input[1], row_to = input[3];

    return {transform(row_from, col_from), transform(row_to, col_to)};
}

Chess::Logic::FigureType TestInput::promote_figure(Chess::Logic::Player *player, Chess::Logic::Position pos) {
    char type;
    std::cin >> type;

    switch (type) {
    case 'B':
        return Chess::Logic::FigureType::Bishop;
    case 'N':
        return Chess::Logic::FigureType::Knight;
    case 'R':
        return Chess::Logic::FigureType::Rook;
    default:
        return Chess::Logic::FigureType::Queen;
    }
}

Chess::Logic::Position TestInput::transform(int row, char col) {
    return {row - 49, (int)col - 97};
}
