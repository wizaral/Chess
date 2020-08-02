#include "definitions.hpp"
#include "factory.hpp"
#include "strategy.hpp"

namespace Chess {

void ClassicFactory::init_game(Logic::Board &board, int &player_index, Publisher *publisher) {
    player_index = 0;
    board.clear_figures();

    board.reset_move_state(Logic::FigureColor::White);
    board.reset_move_state(Logic::FigureColor::Black);

    board.reset_check_state(Logic::FigureColor::White);
    board.reset_check_state(Logic::FigureColor::Black);

    spawn_figures(board, Logic::white_figures_row, Logic::FigureColor::White);
    spawn_figures(board, Logic::black_figures_row, Logic::FigureColor::Black);

    spawn_pawns(board, Logic::white_pawns_row, Logic::FigureColor::White, publisher);
    spawn_pawns(board, Logic::black_pawns_row, Logic::FigureColor::Black, publisher);
}

void ClassicFactory::spawn_pawns(Logic::Board &board, int row, Logic::FigureColor color, Publisher *publisher) {
    for (int i = 0; i < Logic::board_rows; ++i) {
        board.add_figure({Logic::FigureType::Pawn, color, std::make_unique<Logic::PawnStrategy>(publisher, color)}, {row, i});
    }
}

void ClassicFactory::spawn_figures(Logic::Board &board, int row, Logic::FigureColor color) {
    board.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::RookStrategy>()}, {row, 0});
    board.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::KnightStrategy>()}, {row, 1});
    board.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::BishopStrategy>()}, {row, 2});
    board.add_figure({Logic::FigureType::Queen, color, std::make_unique<Logic::QueenStrategy>()}, {row, 3});
    board.add_figure({Logic::FigureType::King, color, std::make_unique<Logic::KingStrategy>()}, {row, 4});
    board.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::BishopStrategy>()}, {row, 5});
    board.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::KnightStrategy>()}, {row, 6});
    board.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::RookStrategy>()}, {row, 7});
}

}
