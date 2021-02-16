#include "definitions.hpp"
#include "factory.hpp"
#include "figure_bishop.hpp"
#include "figure_king.hpp"
#include "figure_knight.hpp"
#include "figure_pawn.hpp"
#include "figure_queen.hpp"
#include "figure_rook.hpp"

namespace Chess {

void ClassicFactory::init_game(Board &board, int &player_index, Publisher *publisher) {
    player_index = 0;
    board.clear_figures();

    board.reset_move_state(FigureColor::White);
    board.reset_move_state(FigureColor::Black);

    board.reset_check_state(FigureColor::White);
    board.reset_check_state(FigureColor::Black);

    spawn_figures(board, figures_row[static_cast<int>(FigureColor::White)], FigureColor::White);
    spawn_figures(board, figures_row[static_cast<int>(FigureColor::Black)], FigureColor::Black);

    spawn_pawns(board, pawns_row[static_cast<int>(FigureColor::White)], FigureColor::White, publisher);
    spawn_pawns(board, pawns_row[static_cast<int>(FigureColor::Black)], FigureColor::Black, publisher);
}

void ClassicFactory::spawn_pawns(Board &board, int row, FigureColor color, Publisher *publisher) {
    for (int i = 0; i < board_rows; ++i) {
        board.add_figure(std::make_unique<Pawn>(FigureType::Pawn, color, publisher), {row, i});
    }
}

void ClassicFactory::spawn_figures(Board &board, int row, FigureColor color) {
    board.add_figure(std::make_unique<Rook>(FigureType::Rook, color), {row, 0});
    board.add_figure(std::make_unique<Knight>(FigureType::Knight, color), {row, 1});
    board.add_figure(std::make_unique<Bishop>(FigureType::Bishop, color), {row, 2});
    board.add_figure(std::make_unique<Queen>(FigureType::Queen, color), {row, 3});
    board.add_figure(std::make_unique<King>(FigureType::King, color), {row, 4});
    board.add_figure(std::make_unique<Bishop>(FigureType::Bishop, color), {row, 5});
    board.add_figure(std::make_unique<Knight>(FigureType::Knight, color), {row, 6});
    board.add_figure(std::make_unique<Rook>(FigureType::Rook, color), {row, 7});
}

} // namespace Chess
