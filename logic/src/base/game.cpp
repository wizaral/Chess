#include "game.hpp"

namespace Chess {

Game::Game(std::array<std::unique_ptr<Logic::Player>, 2> arr,
            std::unique_ptr<Render> render,
            std::unique_ptr<DataInput> input)
    : players_(std::move(arr))
    , render_(std::move(render))
    , input_(std::move(input)) {
    validate();
    init_game();
}

void Game::spawn_pawns(int row, Logic::FigureColor color, int direction) {
    for (int i = 0; i < Logic::board_rows; ++i) {
        board_.add_figure({Logic::FigureType::Pawn, color, std::make_unique<Logic::Strategy>(Logic::PawnStrategy(this, direction))}, {row, i});
        subscribe(static_cast<Logic::PawnStrategy *>(board_.get_figure({row, i})->strategy()));
    }
}

void Game::spawn_figures(int row, Logic::FigureColor color) {
    board_.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::Strategy>(Logic::RookStrategy())}, {row, 0});
    board_.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::Strategy>(Logic::RookStrategy())}, {row, 7});

    board_.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::Strategy>(Logic::KnightStrategy())}, {row, 1});
    board_.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::Strategy>(Logic::KnightStrategy())}, {row, 6});

    board_.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::Strategy>(Logic::BishopStrategy())}, {row, 2});
    board_.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::Strategy>(Logic::BishopStrategy())}, {row, 5});
}

void Game::loop() {
    Logic::GameState gstate = Logic::GameState::NormalMove;

    for (; gstate != Logic::GameState::CheckMate && gstate != Logic::GameState::Draw; ++player_index_) {
        Logic::Move m = input_->get_next_move(get_current_player());
        gstate = logic(m);

        if (is_error(gstate)) {
            render_->show_error(gstate);
            --player_index_;
        }
        render_->show_board(board_);
    }

    ++player_index_;
    render_->show_endgame(gstate, get_current_player());
}

void Game::subscribe(Subscriber *sub) {
    subs_.push_back(sub);
}

void Game::unsubscribe(Subscriber *sub) {
    subs_.remove(sub);
}

void Game::notify() {
    for (auto i : subs_) {
        i->update();
    }
}

} // namespace Chess
