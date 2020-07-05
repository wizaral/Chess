#include "position.hpp"

namespace Chess::Logic {

Position::Position(int row, int col)
    : row_(row), col_(col) {}

int Position::row() const {
    return row_;
}

int Position::col() const {
    return col_;
}

void Position::validation(const Position &pos) {
    if (int row = pos.row(); row < 0 || row >= board_rows) {
        throw std::out_of_range("Row position isn't valid");
    }
    if (int col = pos.col(); col < 0 || col >= board_cols) {
        throw std::out_of_range("Col position isn't valid");
    }
}

} // namespace Chess::Logic
