#include "strategy.hpp"

namespace Chess::Logic {

GameState RookStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    int rows = move.rows();
    int cols = move.cols();

    if ((rows > 0 && cols > 0) || (rows == 0 && cols == 0))
        return GameState::WrongFigureMove;

    int pos_row = move.from().row();
    int pos_col = move.from().col();

    int row_inc = rows != 0 ? (pos_row < move.to().row() ? 1 : -1) : 0;
    int col_inc = cols != 0 ? (pos_col < move.to().col() ? 1 : -1) : 0;

    while (true) {
        pos_row += row_inc;
        pos_col += col_inc;

        if (move.to().row() == pos_row && move.to().col() == pos_col) {
            Figure *other = board.get_figure(move.to());

            if (other != nullptr && other->color() == figure.color())
                return GameState::OtherFigureOnPath;

            state_ = MoveState::NormalMove;
            return GameState::NormalMove;
        }

        if (board.get_figure({pos_row, pos_col}) != nullptr)
            return GameState::OtherFigureOnPath;
    }
}

RookStrategy::MoveState RookStrategy::state() const {
    return state_;
}

void RookStrategy::castling_update() {
    state_ = MoveState::NormalMove;
}

void RookStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    for (int r = row + 1; r < board_rows; ++r) {
        coords.emplace_back(r, col);

        if (board.get_figure({r, col}) != nullptr) {
            break;
        }
    }

    for (int r = row - 1; r >= 0; --r) {
        coords.emplace_back(r, col);

        if (board.get_figure({r, col}) != nullptr) {
            break;
        }
    }

    for (int c = col + 1; c < board_cols; ++c) {
        coords.emplace_back(row, c);

        if (board.get_figure({row, c}) != nullptr) {
            break;
        }
    }

    for (int c = col - 1; c >= 0; --c) {
        coords.emplace_back(row, c);

        if (board.get_figure({row, c}) != nullptr) {
            break;
        }
    }
}

} // namespace Chess::Logic
