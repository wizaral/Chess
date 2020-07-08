#include "strategy.hpp"

namespace Chess::Logic {

bool QueenStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 0 && cols == 0)
        return false;

    if (rows == cols) {
        int row_inc = move.from().row() < move.to().row() ? 1 : -1;
        int col_inc = move.from().col() < move.to().col() ? 1 : -1;
        return validate(figure, board, move, row_inc, col_inc);
    }
    if ((rows > 0 && cols == 0) || (rows == 0 && cols > 0)) {
        int row_inc = rows != 0 ? (move.from().row() < move.to().row() ? 1 : -1) : 0;
        int col_inc = cols != 0 ? (move.from().col() < move.to().col() ? 1 : -1) : 0;
        return validate(figure, board, move, row_inc, col_inc);
    }

    return false;
}

bool QueenStrategy::validate(const Figure &figure, const Board &board, const Move &move, int row_inc, int col_inc) {
    int pos_row = move.from().row();
    int pos_col = move.from().col();

    while (true) {
        pos_row += row_inc;
        pos_col += col_inc;

        if (move.to().row() == pos_row && move.to().col() == pos_col) {
            Figure *other = board.get_figure(move.to());

            if (other != nullptr && other->color() == figure.color())
                return false;
            return true;
        }

        if (board.get_figure({pos_row, pos_col}) != nullptr)
            return false;
    }
    return false;
}

} // namespace Chess::Logic
