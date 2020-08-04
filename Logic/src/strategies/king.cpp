#include "strategy.hpp"

namespace Chess {

GameState KingStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) const {
    Figure *other = board.get_figure(move.to());
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 1 || cols == 1) {
        if (other == nullptr || other->color() != figure.color()) {
            return GameState::NormalMove;
        }
    } else if (other != nullptr && other->type() == FigureType::Rook && other->color() == figure.color()) {
        return check_castling(figure, board, move, other);
    }
    return GameState::WrongFigureMove;
}

GameState KingStrategy::check_castling(const Figure &figure, const Board &board, const Move &move, Figure *other) const {
    FigureColor other_color = !figure.color();

    // check first figure move
    if (state_ == MoveState::NoMove && (static_cast<RookStrategy *>(other->strategy()))->state() == RookStrategy::MoveState::NoMove) {
        const std::array<std::array<bool, board_rows>, board_cols> &state = board.get_check_state(other_color);
        int distance = move.cols();
        int row = move.from().row();

        if (state[row][4] == true) {
            return GameState::CastlingKingInCheck;
        }

        if (distance == 3) {
            for (int col = 5; col < 7; ++col) {
                if (state[row][col] == true) {
                    return GameState::CastlingSquareInCheck;
                }
                if (board.get_figure({row, col}) != nullptr) {
                    return GameState::CastlingFigureOnPath;
                }
            }
            return GameState::KingCastling;
        } else /* if (distance == 4) */ {
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

void KingStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    if (row + 1 < board_rows && col + 1 < board_cols) {
        coords.emplace_back(row + 1, col + 1);
    }
    if (row + 1 < board_rows && col - 1 >= 0) {
        coords.emplace_back(row + 1, col - 1);
    }
    if (row - 1 >= 0 && col + 1 < board_cols) {
        coords.emplace_back(row - 1, col + 1);
    }
    if (row - 1 >= 0 && col - 1 >= 0) {
        coords.emplace_back(row - 1, col - 1);
    }

    if (row + 1 < board_rows) {
        coords.emplace_back(row + 1, col);
    }
    if (row - 1 >= 0) {
        coords.emplace_back(row - 1, col);
    }
    if (col + 1 < board_cols) {
        coords.emplace_back(row, col + 1);
    }
    if (col - 1 >= 0) {
        coords.emplace_back(row, col - 1);
    }
}

void KingStrategy::update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void KingStrategy::move_update(const Move &move) {
    state_ = MoveState::NormalMove;
}

} // namespace Chess
