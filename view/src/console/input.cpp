#include <iostream>

#include "c_input.hpp"

Chess::Logic::Move ConsoleInput::get_next_move(Chess::Logic::Player *player) {
    std::string input;

    // std::cin.ignore();
    std::cin >> input;

    char col_from = input[0], col_to = input[2];
    int row_from = input[1], row_to = input[3];

    // std::cout << input << ": " << col_from << row_from << "-" << col_to << row_to << std::endl;

    return {transform(row_from, col_from), transform(row_to, col_to)};
}

Chess::Logic::FigureType ConsoleInput::promote_figure(Chess::Logic::Player *player, Chess::Logic::Position pos) {
    char type;
    // std::cin.ignore();
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

Chess::Logic::Position ConsoleInput::transform(int row, char col) {
    return {56 - row, (int)col - 97};
}
