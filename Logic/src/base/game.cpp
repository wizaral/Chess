#include "chess.hpp"

namespace Chess {

ChessGame::ChessGame(std::array<std::unique_ptr<Logic::Player>, 2> arr,
            std::unique_ptr<FigureFactory> factory,
            std::unique_ptr<Render> render,
            std::unique_ptr<DataInput> input,
            std::function<bool()> condition)
    : players_(std::move(arr))
    , factory_(std::move(factory))
    , render_(std::move(render))
    , input_(std::move(input))
    , condition_(condition) {
    validate();
}

void ChessGame::init_game() {
    factory_->init_game(board_, player_index_, this);
}

void ChessGame::validate() const {
    if (players_[0]->color() == players_[1]->color()) {
        throw std::logic_error("Players have same figure colors");
    }
}

void ChessGame::transform_pawn(Logic::Position pos) {
    render_->show_pawn_promotion(board_, pos);
    Logic::FigureType ft = input_->promote_figure(get_current_player(), pos);
    Logic::FigureColor fc = get_current_player()->color();

    switch (ft) {
    case Logic::FigureType::Queen:
        board_.add_figure({ft, fc, std::make_unique<Logic::QueenStrategy>()}, pos);
        break;
    case Logic::FigureType::Rook:
        board_.add_figure({ft, fc, std::make_unique<Logic::RookStrategy>()}, pos);
        break;
    case Logic::FigureType::Knight:
        board_.add_figure({ft, fc, std::make_unique<Logic::KnightStrategy>()}, pos);
        break;
    case Logic::FigureType::Bishop:
        board_.add_figure({ft, fc, std::make_unique<Logic::BishopStrategy>()}, pos);
        break;
    default:
        break;
    }
}

void ChessGame::loop() {
    Logic::GameState gstate = Logic::GameState::NormalMove;

    for (; condition_() && !is_endgame(gstate); ++player_index_) {
        render_->show_board(board_);

        render_->show_next_step(get_current_player());
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
    for (auto it = subs_.begin(); it != subs_.end();) {
        if ((*it)->update(Subscriber::MessageType::Notify) == false) {
            it = subs_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace Chess
