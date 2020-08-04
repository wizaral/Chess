#include <iostream>

#include "t_input.hpp"

Chess::Move TestInput::get_next_move(Chess::Player *player) {
    std::string input;
    std::cin >> input;

    char col_from = input[0], col_to = input[2];
    int row_from = input[1], row_to = input[3];

    return {transform(row_from, col_from), transform(row_to, col_to)};
}

Chess::FigureType TestInput::promote_figure(Chess::Player *player, Chess::Position pos) {
    char type;
    std::cin >> type;

    switch (type) {
    case 'B':
        return Chess::FigureType::Bishop;
    case 'N':
        return Chess::FigureType::Knight;
    case 'R':
        return Chess::FigureType::Rook;
    default:
        return Chess::FigureType::Queen;
    }
}

Chess::Position TestInput::transform(int row, char col) {
    return {row - 49, (int)col - 97};
}
