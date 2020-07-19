#include "definitions.hpp"
#include "factory.hpp"
#include "strategy.hpp"

namespace Chess {

void ClassicFactory::init_game(Logic::Board &board, int &player_index, const std::array<std::unique_ptr<Logic::Player>, 2> &players, Publisher *publisher) {
    Logic::FigureColor p0c = players[0]->color(), p1c = players[1]->color();
    player_index = 0;
    board.clear_figures();
    board.reset_state(Logic::FigureColor::White);
    board.reset_state(Logic::FigureColor::Black);

    if (p0c == Logic::FigureColor::White) {
        board.add_figure({Logic::FigureType::Queen, Logic::FigureColor::White, std::make_unique<Logic::QueenStrategy>()}, {Logic::player0_figures_row, 3});
        board.add_figure({Logic::FigureType::King, Logic::FigureColor::White, std::make_unique<Logic::KingStrategy>()}, {Logic::player0_figures_row, 4});
        board.add_figure({Logic::FigureType::Queen, Logic::FigureColor::Black, std::make_unique<Logic::QueenStrategy>()}, {Logic::player1_figures_row, 3});
        board.add_figure({Logic::FigureType::King, Logic::FigureColor::Black, std::make_unique<Logic::KingStrategy>()}, {Logic::player1_figures_row, 4});
    } else {
        board.add_figure({Logic::FigureType::Queen, Logic::FigureColor::Black, std::make_unique<Logic::QueenStrategy>()}, {Logic::player0_figures_row, 4});
        board.add_figure({Logic::FigureType::King, Logic::FigureColor::Black, std::make_unique<Logic::KingStrategy>()}, {Logic::player0_figures_row, 3});
        board.add_figure({Logic::FigureType::Queen, Logic::FigureColor::White, std::make_unique<Logic::QueenStrategy>()}, {Logic::player1_figures_row, 4});
        board.add_figure({Logic::FigureType::King, Logic::FigureColor::White, std::make_unique<Logic::KingStrategy>()}, {Logic::player1_figures_row, 3});
    }

    spawn_figures(board, Logic::player0_figures_row, p0c);
    spawn_pawns(board, Logic::player0_pawns_row, p0c, Logic::player0_step_direction, publisher);

    spawn_figures(board, Logic::player1_figures_row, p1c);
    spawn_pawns(board, Logic::player1_pawns_row, p1c, Logic::player1_step_direction, publisher);
}

void ClassicFactory::spawn_pawns(Logic::Board &board, int row, Logic::FigureColor color, int direction, Publisher *publisher) {
    for (int i = 0; i < Logic::board_rows; ++i) {
        board.add_figure({Logic::FigureType::Pawn, color, std::make_unique<Logic::PawnStrategy>(publisher, direction)}, {row, i});
    }
}

void ClassicFactory::spawn_figures(Logic::Board &board, int row, Logic::FigureColor color) {
    board.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::RookStrategy>()}, {row, 0});
    board.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::RookStrategy>()}, {row, 7});

    board.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::KnightStrategy>()}, {row, 1});
    board.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::KnightStrategy>()}, {row, 6});

    board.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::BishopStrategy>()}, {row, 2});
    board.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::BishopStrategy>()}, {row, 5});
}

}
