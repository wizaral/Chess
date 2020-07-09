#include "strategy.hpp"

namespace Chess::Logic {

MoveType KnightStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    int rows = move.rows();
    int cols = move.cols();

    if ((rows == 2 && cols == 1) || (rows == 1 && cols == 2)) {
        Figure *other = board.get_figure(move.to());
        if (other == nullptr || other->color() != figure.color()) {
            return MoveType::True;  // NormalMove
        } else {
            return MoveType::False; // OtherFigureOnPath
        }
    }
    return MoveType::False; // WrongFigureMove
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

} // namespace Chess::Logic
