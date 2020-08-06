#include "strategy.hpp"

namespace Chess {

PawnStrategy::PawnStrategy(Publisher *publisher, FigureColor color)
    : Subscriber(publisher) {
    direction_ = color == FigureColor::White ? white_step_direction : black_step_direction;
}

PawnStrategy::PawnStrategy(Publisher *publisher, FigureColor color, MoveState state)
    : Subscriber(publisher)
    , state_(state) {
    direction_ = color == FigureColor::White ? white_step_direction : black_step_direction;
}

PawnStrategy::MoveState PawnStrategy::state() const {
    return state_;
}

bool PawnStrategy::update(MessageType type) {
    if (type == MessageType::Notify) {
        if (state_ == MoveState::DoubleMove) {
            state_ = MoveState::EnPassant;
        } else if (state_ != MoveState::NoMove) {
            state_ = MoveState::NormalMove;
            publisher_ = nullptr;
            return false;
        }
        return true;
    } else /* if (type MessageType::Destroy ) */ {
        publisher_ = nullptr;
    }
    return false;
}

GameState PawnStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) const {
    Position from = move.from(), to = move.to();

    if ((direction_ > 0 && to.row() < from.row()) || (direction_ < 0 && to.row() > from.row())) {
        return GameState::PawnStepBack;
    } else if (to.row() == from.row()) {
        return GameState::WrongFigureMove;
    }

    Figure *other = board.get_figure(to);
    FigureColor other_color = !figure.color();

    if (from.row() + direction_ == to.row() && check_diagonal(move)) {
        if (other != nullptr) {
            if (other->color() == other_color) {
                return GameState::NormalMove;
            } else {
                return GameState::OtherFigureOnPath;
            }
        } else /* if (other == nullptr) */ {
            // En passant
            return check_pawn(board.get_figure({from.row(), to.col()}), other_color);
        }
    }
    // double move
    if (state_ == MoveState::NoMove && from.col() == to.col() && from.row() + 2 * direction_ == to.row() && other == nullptr) {
        // check figure between start point and destination
        if (board.get_figure({from.row() + direction_, to.col()}) != nullptr) {
            return GameState::OtherFigureOnPath;
        }
        return GameState::NormalMove;
    }
    // standard move
    if (from.row() + direction_ == to.row() && from.col() == to.col()) {
        if (other == nullptr) {
            return GameState::NormalMove;
        } else {
            return GameState::OtherFigureOnPath;
        }
    }
    return GameState::WrongFigureMove;
}

void PawnStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int dest = pos.row() + direction_;
    int col = pos.col();

    if (Position::validation({dest, col + 1})) {
        coords.emplace_back(dest, col + 1);
    }
    if (Position::validation({dest, col - 1})) {
        coords.emplace_back(dest, col - 1);
    }
}

void PawnStrategy::update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    std::array<Position, 4> positions{
        Position{row + direction_, col + 1},
        Position{row + direction_, col - 1},
        Position{row + 2 * direction_, col},
        Position{row + direction_, col},
    };

    for (auto i : positions) {
        if (Position::validation(i)) {
            auto valid = validate_move(figure, board, {pos, i});

            if (valid == GameState::NormalMove || valid == GameState::EnPassant) {
                coords.push_back(i);
            }
        }
    }
}

void PawnStrategy::move_update(const Move &move) {
    state_ = move.rows() > 1 ? MoveState::DoubleMove : MoveState::NormalMove;
}

GameState PawnStrategy::check_pawn(Figure *figure, FigureColor color) const {
    if (figure != nullptr && figure->color() == color && figure->type() == FigureType::Pawn) {
        if (static_cast<PawnStrategy *>(figure->strategy())->state() == PawnStrategy::MoveState::EnPassant) {
            return GameState::EnPassant;
        }
        return GameState::FailEnPassant;
    }
    return GameState::WrongFigureMove;
}

bool PawnStrategy::check_diagonal(const Move &move) const {
    return move.from().col() + 1 == move.to().col() || move.from().col() - 1 == move.to().col();
}

} // namespace Chess
