#include "board.hpp"
#include "figure.hpp"
#include "move.hpp"
#include "strategy.hpp"

namespace Chess::Logic {

PawnStrategy::PawnStrategy(Publisher *publisher)
    : Subscriber(publisher) {}

PawnStrategy::MoveState PawnStrategy::state() const {
    return state_;
}

void PawnStrategy::update() {
    if (state_ != MoveState::NoMove) {
        state_ = MoveState::NormalMove;
        publisher_->unsubscribe(this);
        publisher_ = nullptr;
    }
}

bool PawnStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    FigureColor this_color = figure.color();
    FigureColor other_color = this_color == FigureColor::Light ? FigureColor::Dark : FigureColor::Light;
    Position from = move.from(), to = move.to();
    Figure *other = board.get_figure(to);

    if (this_color == FigureColor::Light && to.row() <= from.row())
        return false;
    if (this_color == FigureColor::Dark && to.row() >= from.row())
        return false;

    if (this_color == FigureColor::Light) {
        if (from.row() + 1 == to.row() && check_diagonal(move)) {
            if (other != nullptr && other->color() == other_color) {
                state_ = MoveState::NormalMove;
                return true;
            }
            // En passant
            if (other == nullptr && check_pawn(board.get_figure({to.row() - 1, to.col()}), other_color)) {
                state_ = MoveState::NormalMove;
                return true;
            }
        }
        if (state_ == MoveState::NoMove && from.col() == to.col() && from.row() + 2 == to.row() && other == nullptr) {
            state_ = MoveState::DoubleMove;
            return true;
        }
        if (from.row() + 1 == to.row() && from.col() == to.col() && other == nullptr) {
            state_ = MoveState::NormalMove;
            return true;
        }
    } else {
        if (from.row() - 1 == to.row() && check_diagonal(move)) {
            if (other != nullptr && other->color() == other_color) {
                state_ = MoveState::NormalMove;
                return true;
            }
            // En passant
            if (other == nullptr && check_pawn(board.get_figure({to.row() + 1, to.col()}), other_color)) {
                state_ = MoveState::NormalMove;
                return true;
            }
        }
        if (state_ == MoveState::NoMove && from.col() == to.col() && from.row() - 2 == to.row() && other == nullptr) {
            state_ = MoveState::DoubleMove;
            return true;
        }
        if (from.row() - 1 == to.row() && from.col() == to.col() && other == nullptr) {
            state_ = MoveState::NormalMove;
            return true;
        }
    }
    return false;
}

bool PawnStrategy::update_occupation(const Figure &figure, const Board &board, const Position &pos) const {
    return true;
}

bool check_pawn(Figure *figure, FigureColor color) {
    if (figure != nullptr && figure->color() == color && figure->type() == FigureType::Pawn) {
        PawnStrategy *pawn_strategy = static_cast<PawnStrategy *>(figure->strategy());
        return pawn_strategy->state() == PawnStrategy::MoveState::DoubleMove;
    }
    return false;
}

bool PawnStrategy::check_diagonal(const Move &move) {
    return move.from().col() + 1 == move.to().col() || move.from().col() - 1 == move.to().col();
}

} // namespace Chess::Logic
