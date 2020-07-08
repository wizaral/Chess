#include "strategy.hpp"

namespace Chess::Logic {

bool KingStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 1 || cols == 1) {
        Figure *other = board.get_figure(move.to());
        if (other == nullptr || other->color() != figure.color()) {
            state_ = MoveState::NormalMove;
            return true;
        }
    }
    return false;
}

} // namespace Chess::Logic
