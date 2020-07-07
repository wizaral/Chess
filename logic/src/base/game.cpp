#include "game.hpp"

namespace Chess::Logic {

Game::Game(std::array<std::unique_ptr<Player>, 2> arr)
    : players_(std::move(arr)) {
    validate();
    init_game();
}

void Game::init_game() {
    FigureColor p0c = players_[0]->color(), p1c = players_[1]->color();

    spawn_figures(player0_figures_row, p0c);
    spawn_pawns(player0_pawns_row, p0c);

    spawn_figures(player1_figures_row, p1c);
    spawn_pawns(player1_pawns_row, p1c);

    if (p0c == FigureColor::Light) {
        board_.add_figure({FigureType::Queen, FigureColor::Light, std::make_unique<Strategy>(QueenStrategy())}, {player0_figures_row, 3});
        board_.add_figure({FigureType::King, FigureColor::Light, std::make_unique<Strategy>(KingStrategy())}, {player0_figures_row, 4});
        board_.add_figure({FigureType::Queen, FigureColor::Dark, std::make_unique<Strategy>(QueenStrategy())}, {player1_figures_row, 3});
        board_.add_figure({FigureType::King, FigureColor::Dark, std::make_unique<Strategy>(KingStrategy())}, {player1_figures_row, 4});
    } else {
        board_.add_figure({FigureType::Queen, FigureColor::Dark, std::make_unique<Strategy>(QueenStrategy())}, {player0_figures_row, 4});
        board_.add_figure({FigureType::King, FigureColor::Dark, std::make_unique<Strategy>(KingStrategy())}, {player0_figures_row, 3});
        board_.add_figure({FigureType::Queen, FigureColor::Light, std::make_unique<Strategy>(QueenStrategy())}, {player1_figures_row, 4});
        board_.add_figure({FigureType::King, FigureColor::Light, std::make_unique<Strategy>(KingStrategy())}, {player1_figures_row, 3});
    }
}

void Game::loop() {

}

void Game::spawn_pawns(int row, FigureColor color) {
    for (int i = 0; i < board_rows; ++i) {
        board_.add_figure({FigureType::Pawn, color, std::make_unique<Strategy>(PawnStrategy(this))}, {row, i});
    }
}

void Game::spawn_figures(int row, FigureColor color) {
    board_.add_figure({FigureType::Rook, color, std::make_unique<Strategy>(RookStrategy())}, {row, 0});
    board_.add_figure({FigureType::Rook, color, std::make_unique<Strategy>(RookStrategy())}, {row, 7});

    board_.add_figure({FigureType::Knight, color, std::make_unique<Strategy>(KnightStrategy())}, {row, 1});
    board_.add_figure({FigureType::Knight, color, std::make_unique<Strategy>(KnightStrategy())}, {row, 6});

    board_.add_figure({FigureType::Bishop, color, std::make_unique<Strategy>(BishopStrategy())}, {row, 2});
    board_.add_figure({FigureType::Bishop, color, std::make_unique<Strategy>(BishopStrategy())}, {row, 5});
}

void Game::validate() const {
    if (players_[0]->color() == players_[1]->color())
        throw(std::logic_error("Players have same figure colors"));
}

} // namespace Chess::Logic
