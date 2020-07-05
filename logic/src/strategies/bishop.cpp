#include <cmath>

#include "board.hpp"
#include "figure.hpp"
#include "move.hpp"
#include "strategy.hpp"

namespace Chess::Logic {

bool BishopStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    int rows = abs(move.from().row() - move.to().row());
    int cols = abs(move.from().col() - move.to().col());

    if (rows != cols)
        return false;

    int pos_row = move.from().row();
    int pos_col = move.from().col();

    int row_inc = pos_row < move.to().row() ? 1 : -1;
    int col_inc = pos_col < move.to().col() ? 1 : -1;

    while (true) {
        pos_row += row_inc;
        pos_col += col_inc;

        if (move.to().row() == pos_row && move.to().col() == pos_col) {
            auto other = board.get_figure(move.to());

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
