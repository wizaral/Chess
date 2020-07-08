#include "strategy.hpp"

namespace Chess::Logic {

bool KnightStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    int rows = move.rows();
    int cols = move.cols();

    if ((rows == 2 && cols == 1) || (rows == 1 && cols == 2)) {
        Figure *other = board.get_figure(move.to());
        if (other == nullptr || other->color() != figure.color()) {
            return true;
        }
    }
    return false;
}

} // namespace Chess::Logic
