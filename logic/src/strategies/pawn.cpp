#include "strategy.hpp"

namespace Chess::Logic {

PawnStrategy::PawnStrategy(Publisher *publisher, int direction)
    : Subscriber(publisher), direction_(direction) {}

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

GameState PawnStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    Position from = move.from(), to = move.to();

    if (direction_ > 0 && to.row() <= from.row())
        return GameState::PawnStepBack;
    if (direction_ < 0 && to.row() >= from.row())
        return GameState::PawnStepBack;

    Figure *other = board.get_figure(to);
    FigureColor other_color = !figure.color();

    if (from.row() + direction_ == to.row() && check_diagonal(move)) {
        if (other != nullptr) {
            if (other->color() == other_color) {
                state_ = MoveState::NormalMove;
                return GameState::NormalMove;
            } else {
                return GameState::OtherFigureOnPath;
            }
        }
        // En passant
        if (other == nullptr) {
            GameState gs = check_pawn(board.get_figure({to.row() - direction_, to.col()}), other_color);

            if (gs == GameState::NormalMove) {
                state_ = MoveState::NormalMove;
            }
            return gs;
        }
    }
    if (state_ == MoveState::NoMove && from.col() == to.col() && from.row() + 2 * direction_ == to.row() && other == nullptr) {
        if (board.get_figure({from.row() + direction_, to.col()}) != nullptr) {
            return GameState::OtherFigureOnPath;
        }
        state_ = MoveState::DoubleMove;
        return GameState::NormalMove;
    }
    if (from.row() + direction_ == to.row() && from.col() == to.col()) {
        if (other == nullptr) {
            state_ = MoveState::NormalMove;
            return GameState::NormalMove;
        } else {
            return GameState::OtherFigureOnPath;
        }
    }
    return GameState::WrongFigureMove;
}

void PawnStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    if (row + direction_ < board_rows && row + direction_ >= 0) {
        if (col + 1 < board_cols) {
            coords.emplace_back(row + direction_, col + 1);
        }
        if (col - 1 >= 0) {
            coords.emplace_back(row + direction_, col - 1);
        }
    }
}

GameState PawnStrategy::check_pawn(Figure *figure, FigureColor color) {
    if (figure != nullptr && figure->color() == color && figure->type() == FigureType::Pawn) {
        PawnStrategy *pawn_strategy = static_cast<PawnStrategy *>(figure->strategy());
        if (pawn_strategy->state() == PawnStrategy::MoveState::EnPassant) {
            return GameState::NormalMove;
        }
        return GameState::FailEnPassant;
    }
    return GameState::WrongFigureMove;
}

bool PawnStrategy::check_diagonal(const Move &move) {
    return move.from().col() + 1 == move.to().col() || move.from().col() - 1 == move.to().col();
}

} // namespace Chess::Logic
