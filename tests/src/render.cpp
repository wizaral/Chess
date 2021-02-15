#include "t_render.hpp"

void TestRender::show_menu() {
    std::cout << "THIS IS MENU" << std::endl;
}

void TestRender::show_board(const Chess::Board &board) {
    //system("cls");
    std::cout << ' ';
    for (int i = 0; i < Chess::board_cols; ++i) {
        std::cout << "  " << static_cast<char>('A' + i);
    }
    std::cout << '\n';

    for (int row = Chess::board_rows - 1; row > -1; --row) {
        std::cout << row + 1 << "|";

        for (int col = 0; col < Chess::board_cols; ++col) {
            if (auto f = board.get_figure({row, col}); f != nullptr) {
                std::cout << s_colors.at(f->color()) << s_figures.at(f->type()) << '|';
            } else {
                std::cout << "  |";
            }
        }
        std::cout << "\n =========================\n";
    }
    std::cout << std::endl;
}

void TestRender::show_next_step(Chess::Player *player) {
    std::cout << player->name() << " move: ";
}

void TestRender::show_error(Chess::GameState state) {
    std::cout << "Error: " << s_states.at(state) << std::endl;
}

void TestRender::show_pawn_promotion(Chess::Position pos) {
    std::cout << "Enter first letter of figure type name in uppercase for promotion pawn on [" << (8 - pos.row()) << "][" << static_cast<char>('A' + pos.col()) << "]: ";
}

void TestRender::show_endgame(Chess::GameState state, Chess::Player *winer) {
    switch (state) {
    case Chess::GameState::CheckMate:
        std::cout << "Player " << winer->name() << " has won!" << std::endl;
        break;
    case Chess::GameState::StaleMate:
        std::cout << "Stalemate. Game over!" << std::endl;
        break;
    default:
        std::cout << "Draw. Game over!" << std::endl;
        break;
    }
}
