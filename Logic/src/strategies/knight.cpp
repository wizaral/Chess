#include "strategy.hpp"

namespace Chess {

GameState KnightStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) const {
    int rows = move.rows();
    int cols = move.cols();

    if ((rows == 2 && cols == 1) || (rows == 1 && cols == 2)) {
        Figure *other = board.get_figure(move.to());
        if (other == nullptr || other->color() != figure.color()) {
            return GameState::NormalMove;
        } else {
            return GameState::OtherFigureOnPath;
        }
    }
    return GameState::WrongFigureMove;
}

void KnightStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    if (row + 2 < board_rows && col + 1 < board_cols) {
        coords.emplace_back(row + 2, col + 1);
    }
    if (row + 2 < board_rows && col - 1 >= 0) {
        coords.emplace_back(row + 2, col - 1);
    }
    if (row + 1 < board_rows && col + 2 < board_cols) {
        coords.emplace_back(row + 1, col + 2);
    }
    if (row - 1 >= 0 && col + 2 < board_cols) {
        coords.emplace_back(row - 1, col + 2);
    }
    if (row - 2 >= 0 && col + 1 < board_cols) {
        coords.emplace_back(row - 2, col + 1);
    }
    if (row - 2 >= 0 && col - 1 >= 0) {
        coords.emplace_back(row - 2, col - 1);
    }
    if (row + 1 < board_rows && col - 2 >= 0) {
        coords.emplace_back(row + 1, col - 2);
    }
    if (row - 1 >= 0 && col - 2 >= 0) {
        coords.emplace_back(row - 1, col - 2);
    }
}

void KnightStrategy::update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void KnightStrategy::move_update(const Move &move) {}

} // namespace Chess
