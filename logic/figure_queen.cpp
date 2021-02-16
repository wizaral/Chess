#include "board.hpp"
#include "figure_queen.hpp"

namespace Chess {

GameState Queen::validate_move(const Board &board, const Move &move) const {
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 0 && cols == 0) {
        return GameState::WrongFigureMove;
    }

    if (rows == cols) {
        int row_inc = move.from().row() < move.to().row() ? 1 : -1;
        int col_inc = move.from().col() < move.to().col() ? 1 : -1;
        return validate(board, move, row_inc, col_inc);
    }
    if ((rows > 0 && cols == 0) || (rows == 0 && cols > 0)) {
        int row_inc = rows != 0 ? (move.from().row() < move.to().row() ? 1 : -1) : 0;
        int col_inc = cols != 0 ? (move.from().col() < move.to().col() ? 1 : -1) : 0;
        return validate(board, move, row_inc, col_inc);
    }

    return GameState::WrongFigureMove;
}

GameState Queen::validate(const Board &board, const Move &move, int row_inc, int col_inc) const {
    int pos_row = move.from().row();
    int pos_col = move.from().col();

    while (true) {
        pos_row += row_inc;
        pos_col += col_inc;

        if (move.to().row() == pos_row && move.to().col() == pos_col) {
            auto other = board.get_figure(move.to());

            if (other != nullptr && other->color() == color()) {
                return GameState::OtherFigureOnPath;
            }
            return GameState::NormalMove;
        }

        if (board.get_figure({pos_row, pos_col}) != nullptr) {
            return GameState::OtherFigureOnPath;
        }
    }
}

void Queen::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    bishop_occupation(board, pos, coords);
    rook_occupation(board, pos, coords);
}

void Queen::update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void Queen::move_update(const Move &move) {}

void Queen::bishop_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    for (int r = row + 1, c = col + 1; r < board_rows && c < board_cols; ++r, ++c) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row - 1, c = col + 1; r >= 0 && c < board_cols; --r, ++c) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row + 1, c = col - 1; r < board_rows && c >= 0; ++r, --c) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }
}

void Queen::rook_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    for (int r = row + 1, c = col; r < board_rows; ++r) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row - 1, c = col; r >= 0; --r) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row, c = col + 1; c < board_cols; ++c) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row, c = col - 1; c >= 0; --c) {
        coords.emplace_back(r, c);
        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }
}

} // namespace Chess
