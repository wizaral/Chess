#include "strategy.hpp"

namespace Chess::Logic {

GameState BishopStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
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
            Figure *other = board.get_figure(move.to());

            if (other != nullptr && other->color() == figure.color()) {
                return GameState::OtherFigureOnPath;
            }
            return GameState::NormalMove;
        }

        if (board.get_figure({pos_row, pos_col}) != nullptr) {
            return GameState::OtherFigureOnPath;
        }
    }
}

void BishopStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
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

} // namespace Chess::Logic
