#include "board.hpp"
#include "figure_pawn.hpp"

namespace Chess {

Pawn::Pawn(FigureType type, FigureColor color, Publisher *publisher)
: Figure(type, color)
, Subscriber(publisher) {
    m_direction = step_direction[static_cast<int>(color)];
}

Pawn::Pawn(FigureType type, FigureColor color, Publisher *publisher, MoveState state)
: Figure(type, color)
, Subscriber(publisher)
, m_state(state) {
    m_direction = step_direction[static_cast<int>(color)];
}

Pawn::MoveState Pawn::state() const {
    return m_state;
}

bool Pawn::update(MessageType type) {
    if (type == MessageType::Notify) {
        if (m_state == MoveState::DoubleMove) {
            m_state = MoveState::EnPassant;
        } else if (m_state != MoveState::NoMove) {
            m_state = MoveState::NormalMove;
            m_publisher = nullptr;
            return false;
        }
        return true;
    } else /* if (type == MessageType::Destroy) */ {
        m_publisher = nullptr;
    }
    return false;
}

GameState Pawn::validate_move(const Board &board, const Move &move) const {
    Position from = move.from(), to = move.to();

    if ((m_direction > 0 && to.row() < from.row()) || (m_direction < 0 && to.row() > from.row())) {
        return GameState::PawnStepBack;
    } else if (to.row() == from.row()) {
        return GameState::WrongFigureMove;
    }

    auto other = board.get_figure(to);
    auto other_color = !color();

    if (from.row() + m_direction == to.row() && check_diagonal(move)) {
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
    if (m_state == MoveState::NoMove && from.col() == to.col() && from.row() + 2 * m_direction == to.row() && other == nullptr) {
        // check figure between start point and destination
        if (board.get_figure({from.row() + m_direction, to.col()}) != nullptr) {
            return GameState::OtherFigureOnPath;
        }
        return GameState::NormalMove;
    }
    // standard move
    if (from.row() + m_direction == to.row() && from.col() == to.col()) {
        if (other == nullptr) {
            return GameState::NormalMove;
        } else {
            return GameState::OtherFigureOnPath;
        }
    }
    return GameState::WrongFigureMove;
}

void Pawn::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int dest = pos.row() + m_direction;
    int col = pos.col();

    if (Position::validation({dest, col + 1})) {
        coords.emplace_back(dest, col + 1);
    }
    if (Position::validation({dest, col - 1})) {
        coords.emplace_back(dest, col - 1);
    }
}

void Pawn::update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    std::array<Position, 4> positions{
        Position{row + m_direction, col + 1},
        Position{row + m_direction, col - 1},
        Position{row + 2 * m_direction, col},
        Position{row + m_direction, col},
    };

    for (auto i : positions) {
        if (Position::validation(i)) {
            auto valid = validate_move(board, {pos, i});

            if (valid == GameState::NormalMove || valid == GameState::EnPassant) {
                coords.push_back(i);
            }
        }
    }
}

void Pawn::move_update(const Move &move) {
    m_state = move.rows() > 1 ? MoveState::DoubleMove : MoveState::NormalMove;
}

GameState Pawn::check_pawn(const Figure *figure, FigureColor color) const {
    if (figure != nullptr && figure->color() == color && figure->type() == FigureType::Pawn) {
        if (static_cast<const Pawn *>(figure)->state() == Pawn::MoveState::EnPassant) {
            return GameState::EnPassant;
        }
        return GameState::FailEnPassant;
    }
    return GameState::WrongFigureMove;
}

bool Pawn::check_diagonal(const Move &move) const {
    return move.from().col() + 1 == move.to().col() || move.from().col() - 1 == move.to().col();
}

} // namespace Chess
