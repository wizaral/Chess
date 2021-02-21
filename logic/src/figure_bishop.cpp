#include "board.hpp"
#include "figure_bishop.hpp"

namespace Chess {

GameState Bishop::validate_move(const Board &board, const Move &move) const {
    int rows = move.rows();
    int cols = move.cols();

    if (rows != cols || (rows == 0 && cols == 0)) {
        return GameState::WrongFigureMove;
    }

    int pos_row = move.from().row();
    int pos_col = move.from().col();

    int row_inc = pos_row < move.to().row() ? 1 : -1;
    int col_inc = pos_col < move.to().col() ? 1 : -1;

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

void Bishop::update_occupation(const Board &board, Position pos, std::vector<Position> &coords) const {
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

void Bishop::update_movement(const Board &board, Position pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void Bishop::move_update(const Move &move) {}

} // namespace Chess
