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

} // namespace Chess::Logic
