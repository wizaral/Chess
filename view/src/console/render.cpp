#include "c_render.hpp"

void ConsoleRender::show_menu() {
    std::cout << "THIS IS MENU" << std::endl;
}

void ConsoleRender::show_board(const Chess::Logic::Board &board) {
    for (int i = 0; i < Chess::Logic::board_cols; ++i) {
        std::cout << ' ' << (char)('A' + i) << ' ';
    }
    std::cout << '\n';

    for (int row = Chess::Logic::board_rows; row > 0; --row) {
        std::cout << row << " #";

        for (int col = 0; col < Chess::Logic::board_cols; ++col) {
            if (auto f = board.get_figure({7 - row, col}); f != nullptr) {
                std::cout << colors.at(f->color()) << figures.at(f->type()) << '#';
            } else {
                std::cout << "  #";
            }
        }
        std::cout << ' ' << row << "\n  ################\n";
    }
}

void ConsoleRender::show_error(Chess::Logic::GameState state) {
}

void ConsoleRender::show_pawn_promotion(Chess::Logic::Position pos) {
}

void ConsoleRender::show_endgame(Chess::Logic::GameState state, Chess::Logic::Player *winer) {
}
