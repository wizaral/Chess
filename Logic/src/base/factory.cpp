#include "definitions.hpp"
#include "factory.hpp"
#include "strategy.hpp"

namespace Chess {

void ClassicFactory::init_game(Board &board, int &player_index, Publisher *publisher) {
    player_index = 0;
    board.clear_figures();

    board.reset_move_state(FigureColor::White);
    board.reset_move_state(FigureColor::Black);

    board.reset_check_state(FigureColor::White);
    board.reset_check_state(FigureColor::Black);

    spawn_figures(board, white_figures_row, FigureColor::White);
    spawn_figures(board, black_figures_row, FigureColor::Black);

    spawn_pawns(board, white_pawns_row, FigureColor::White, publisher);
    spawn_pawns(board, black_pawns_row, FigureColor::Black, publisher);
}

void ClassicFactory::spawn_pawns(Board &board, int row, FigureColor color, Publisher *publisher) {
    for (int i = 0; i < board_rows; ++i) {
        board.add_figure({FigureType::Pawn, color, std::make_unique<PawnStrategy>(publisher, color)}, {row, i});
    }
}

void ClassicFactory::spawn_figures(Board &board, int row, FigureColor color) {
    board.add_figure({FigureType::Rook, color, std::make_unique<RookStrategy>()}, {row, 0});
    board.add_figure({FigureType::Knight, color, std::make_unique<KnightStrategy>()}, {row, 1});
    board.add_figure({FigureType::Bishop, color, std::make_unique<BishopStrategy>()}, {row, 2});
    board.add_figure({FigureType::Queen, color, std::make_unique<QueenStrategy>()}, {row, 3});
    board.add_figure({FigureType::King, color, std::make_unique<KingStrategy>()}, {row, 4});
    board.add_figure({FigureType::Bishop, color, std::make_unique<BishopStrategy>()}, {row, 5});
    board.add_figure({FigureType::Knight, color, std::make_unique<KnightStrategy>()}, {row, 6});
    board.add_figure({FigureType::Rook, color, std::make_unique<RookStrategy>()}, {row, 7});
}

}
