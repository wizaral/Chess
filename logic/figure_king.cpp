#include "figure_king.hpp"

namespace Chess {

GameState King::validate_move(const Board &board, const Move &move) const {
    Figure *other = board.get_figure(move.to());
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 1 || cols == 1) {
        if (other == nullptr || other->color() != color()) {
            return GameState::NormalMove;
        }
        return GameState::OtherFigureOnPath;
    }
    if (other == nullptr && rows == 0 && cols == 2) {
        other = board.get_figure({move.from().row(), move.from().col() < move.to().col() ? 7 : 0});
        if (other != nullptr) {
            return check_castling(board, move, other);
        }
    }
    return GameState::WrongFigureMove;
}

GameState King::check_castling(const Board &board, const Move &move, Figure *other) const {
    FigureColor other_color = !color();

    // check first figure move
    if (m_state == MoveState::NoMove && static_cast<Rook *>(other)->state() == Rook::MoveState::NoMove) {
        const std::array<std::array<bool, board_rows>, board_cols> &state = board.get_check_state(other_color);
        int row = move.from().row();

        if (state[row][4] == true) {
            return GameState::CastlingKingInCheck;
        }

        if (int distance = move.from().col() - move.to().col(); distance < 0) {
            for (int col = 5; col < 7; ++col) {
                if (state[row][col] == true) {
                    return GameState::CastlingSquareInCheck;
                }
                if (board.get_figure({row, col}) != nullptr) {
                    return GameState::CastlingFigureOnPath;
                }
            }
            return GameState::KingCastling;
        } else /* if (distance > 0) */ {
            for (int col = 3; col > 1; --col) {
                if (state[row][col] == true) {
                    return GameState::CastlingSquareInCheck;
                }
                if (board.get_figure({row, col}) != nullptr) {
                    return GameState::CastlingFigureOnPath;
                }
            }
            return board.get_figure({row, 1}) == nullptr ? GameState::QueenCastling : GameState::CastlingFigureOnPath;
        }
    }
    return GameState::FiguresAlreadyMoved;
}

void King::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    if (Position::validation({row + 1, col + 1})) {
        coords.emplace_back(row + 1, col + 1);
    }
    if (Position::validation({row + 1, col - 1})) {
        coords.emplace_back(row + 1, col - 1);
    }
    if (Position::validation({row - 1, col + 1})) {
        coords.emplace_back(row - 1, col + 1);
    }
    if (Position::validation({row - 1, col - 1})) {
        coords.emplace_back(row - 1, col - 1);
    }

    if (Position::validation({row + 1, col})) {
        coords.emplace_back(row + 1, col);
    }
    if (Position::validation({row - 1, col})) {
        coords.emplace_back(row - 1, col);
    }
    if (Position::validation({row, col + 1})) {
        coords.emplace_back(row, col + 1);
    }
    if (Position::validation({row, col - 1})) {
        coords.emplace_back(row, col - 1);
    }
}

void King::update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void King::move_update(const Move &move) {
    m_state = MoveState::NormalMove;
}

} // namespace Chess
