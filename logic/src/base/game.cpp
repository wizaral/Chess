#include "chess.hpp"

namespace Chess {

ChessGame::ChessGame(std::array<std::unique_ptr<Logic::Player>, 2> arr,
            std::unique_ptr<Render> render,
            std::unique_ptr<DataInput> input)
    : players_(std::move(arr))
    , render_(std::move(render))
    , input_(std::move(input)) {
    validate();
    init_game();
}

void ChessGame::init_game() {
    Logic::FigureColor player0_color = players_[0]->color(), p1c = players_[1]->color();

    if (player0_color == Logic::FigureColor::Light) {
        board_.add_figure({Logic::FigureType::Queen, Logic::FigureColor::Light, std::make_unique<Logic::Strategy>(Logic::QueenStrategy())}, {Logic::player0_figures_row, 3});
        board_.add_figure({Logic::FigureType::King, Logic::FigureColor::Light, std::make_unique<Logic::Strategy>(Logic::KingStrategy())}, {Logic::player0_figures_row, 4});
        board_.add_figure({Logic::FigureType::Queen, Logic::FigureColor::Dark, std::make_unique<Logic::Strategy>(Logic::QueenStrategy())}, {Logic::player1_figures_row, 3});
        board_.add_figure({Logic::FigureType::King, Logic::FigureColor::Dark, std::make_unique<Logic::Strategy>(Logic::KingStrategy())}, {Logic::player1_figures_row, 4});
    } else {
        board_.add_figure({Logic::FigureType::Queen, Logic::FigureColor::Dark, std::make_unique<Logic::Strategy>(Logic::QueenStrategy())}, {Logic::player0_figures_row, 4});
        board_.add_figure({Logic::FigureType::King, Logic::FigureColor::Dark, std::make_unique<Logic::Strategy>(Logic::KingStrategy())}, {Logic::player0_figures_row, 3});
        board_.add_figure({Logic::FigureType::Queen, Logic::FigureColor::Light, std::make_unique<Logic::Strategy>(Logic::QueenStrategy())}, {Logic::player1_figures_row, 4});
        board_.add_figure({Logic::FigureType::King, Logic::FigureColor::Light, std::make_unique<Logic::Strategy>(Logic::KingStrategy())}, {Logic::player1_figures_row, 3});
    }

    spawn_figures(Logic::player0_figures_row, player0_color);
    spawn_pawns(Logic::player0_pawns_row, player0_color, Logic::player0_step_direction);

    spawn_figures(Logic::player1_figures_row, p1c);
    spawn_pawns(Logic::player1_pawns_row, p1c, Logic::player1_step_direction);
}

void ChessGame::validate() const {
    if (players_[0]->color() == players_[1]->color())
        throw std::logic_error("Players have same figure colors");
}

void ChessGame::spawn_pawns(int row, Logic::FigureColor color, int direction) {
    for (int i = 0; i < Logic::board_rows; ++i) {
        board_.add_figure({Logic::FigureType::Pawn, color, std::make_unique<Logic::Strategy>(Logic::PawnStrategy(this, direction))}, {row, i});
        subscribe(static_cast<Logic::PawnStrategy *>(board_.get_figure({row, i})->strategy()));
    }
}

void ChessGame::spawn_figures(int row, Logic::FigureColor color) {
    board_.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::Strategy>(Logic::RookStrategy())}, {row, 0});
    board_.add_figure({Logic::FigureType::Rook, color, std::make_unique<Logic::Strategy>(Logic::RookStrategy())}, {row, 7});

    board_.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::Strategy>(Logic::KnightStrategy())}, {row, 1});
    board_.add_figure({Logic::FigureType::Knight, color, std::make_unique<Logic::Strategy>(Logic::KnightStrategy())}, {row, 6});

    board_.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::Strategy>(Logic::BishopStrategy())}, {row, 2});
    board_.add_figure({Logic::FigureType::Bishop, color, std::make_unique<Logic::Strategy>(Logic::BishopStrategy())}, {row, 5});
}

void ChessGame::transform_pawn(Logic::Position pos) {
    render_->show_pawn_promotion(pos);
    Logic::FigureType ft = input_->promote_figure(get_current_player(), pos);
    Logic::FigureColor fc = get_current_player()->color();

    switch (ft) {
    case Logic::FigureType::Queen:
        board_.add_figure({ft, fc, std::make_unique<Logic::Strategy>(Logic::QueenStrategy())}, pos);
        break;
    case Logic::FigureType::Rook:
        board_.add_figure({ft, fc, std::make_unique<Logic::Strategy>(Logic::RookStrategy())}, pos);
        break;
    case Logic::FigureType::Knight:
        board_.add_figure({ft, fc, std::make_unique<Logic::Strategy>(Logic::KnightStrategy())}, pos);
        break;
    case Logic::FigureType::Bishop:
        board_.add_figure({ft, fc, std::make_unique<Logic::Strategy>(Logic::BishopStrategy())}, pos);
        break;
    }
}

void ChessGame::loop() {
    Logic::GameState gstate = Logic::GameState::NormalMove;

    for (; !is_endgame(gstate); ++player_index_) {
        render_->show_board(board_);

        Logic::Move m = input_->get_next_move(get_current_player());
        gstate = logic(m);

        if (is_error(gstate)) {
            render_->show_error(gstate);
            --player_index_;
        }
    }

    ++player_index_;
    render_->show_board(board_);
    render_->show_endgame(gstate, get_current_player());
}

void ChessGame::subscribe(Subscriber *sub) {
    subs_.push_back(sub);
}

void ChessGame::unsubscribe(Subscriber *sub) {
    subs_.remove(sub);
}

void ChessGame::notify() {
    for (auto i : subs_) {
        i->update();
    }
}

} // namespace Chess
