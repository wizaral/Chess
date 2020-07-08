#include "strategy.hpp"

namespace Chess::Logic {

PawnStrategy::PawnStrategy(Publisher *publisher, int direction)
    : Subscriber(publisher)
    , direction_(direction) {}

PawnStrategy::MoveState PawnStrategy::state() const {
    return state_;
}

void PawnStrategy::update() {
    if (state_ == MoveState::DoubleMove) {
        state_ = MoveState::EnPassant;
    } else if (state_ != MoveState::NoMove) {
        state_ = MoveState::NormalMove;
        publisher_->unsubscribe(this);
        publisher_ = nullptr;
    }
}

MoveType PawnStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    Position from = move.from(), to = move.to();

    if (direction_ > 0 && to.row() <= from.row())
        return MoveType::False;
    if (direction_ < 0 && to.row() >= from.row())
        return MoveType::False;

    Figure *other = board.get_figure(to);
    FigureColor other_color = figure.color() == FigureColor::Light ? FigureColor::Dark : FigureColor::Light;

    if (from.row() + direction_ == to.row() && check_diagonal(move)) {
        if (other != nullptr && other->color() == other_color) {
            state_ = MoveState::NormalMove;
            return MoveType::True;
        }
        // En passant
        if (other == nullptr && check_pawn(board.get_figure({to.row() - direction_, to.col()}), other_color)) {
            state_ = MoveState::NormalMove;
            return MoveType::True;
        }
    }
    if (state_ == MoveState::NoMove && from.col() == to.col() && from.row() + 2 * direction_ == to.row() && other == nullptr) {
        state_ = MoveState::DoubleMove;
        return MoveType::True;
    }
    if (from.row() + direction_ == to.row() && from.col() == to.col() && other == nullptr) {
        state_ = MoveState::NormalMove;
        return MoveType::True;
    }
    return MoveType::False;
}

void PawnStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    if (row + direction_ < board_rows) {
        if (col + 1 < board_cols) {
            coords.emplace_back(row + direction_, col + 1);
        }
        if (col - 1 >= 0) {
            coords.emplace_back(row + direction_, col - 1);
        }
    }
}

bool check_pawn(Figure *figure, FigureColor color) {
    if (figure != nullptr && figure->color() == color && figure->type() == FigureType::Pawn) {
        PawnStrategy *pawn_strategy = static_cast<PawnStrategy *>(figure->strategy());
        return pawn_strategy->state() == PawnStrategy::MoveState::EnPassant;
    }
    return false;
}

bool PawnStrategy::check_diagonal(const Move &move) {
    return move.from().col() + 1 == move.to().col() || move.from().col() - 1 == move.to().col();
}

} // namespace Chess::Logic
