#include "t_render.hpp"

void TestRender::show_menu() {
    std::cout << "THIS IS MENU" << std::endl;
}

void TestRender::show_board(const Chess::Logic::Board &board) {
    //system("cls");
    std::cout << ' ';
    for (int i = 0; i < Chess::Logic::board_cols; ++i) {
        std::cout << "  " << static_cast<char>('A' + i);
    }
    std::cout << '\n';

    for (int row = Chess::Logic::board_rows - 1; row > -1; --row) {
        std::cout << row + 1 << "|";

        for (int col = 0; col < Chess::Logic::board_cols; ++col) {
            if (auto f = board.get_figure({row, col}); f != nullptr) {
                std::cout << colors.at(f->color()) << figures.at(f->type()) << '|';
            } else {
                std::cout << "  |";
            }
        }
        std::cout << "\n =========================\n";
    }
    std::cout << std::endl;
}

void TestRender::show_next_step(Chess::Logic::Player* player) {
    std::cout << player->name() << " move: ";
}

void TestRender::show_error(Chess::Logic::GameState state) {
    std::cout << "Error: " << states.at(state) << std::endl;
}

void TestRender::show_pawn_promotion(Chess::Logic::Position pos) {
    std::cout << "Enter first letter of figure type name in uppercase for promotion pawn on [" << (8 - pos.row()) << "][" << static_cast<char>('A' + pos.col()) << "]: ";
}

void TestRender::show_endgame(Chess::Logic::GameState state, Chess::Logic::Player *winer) {
    switch (state) {
    case Chess::Logic::GameState::CheckMate:
        std::cout << "Player " << winer->name() << " has won!" << std::endl;
        break;
    case Chess::Logic::GameState::StaleMate:
        std::cout << "Stalemate. Game over!" << std::endl;
        break;
    default:
        std::cout << "Draw. Game over!" << std::endl;
        break;
    }
}
