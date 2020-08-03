#include "position.hpp"
#include "definitions.hpp"

namespace Chess::Logic {

Position::Position(int row, int col)
    : row_(row), col_(col) {}

int Position::row() const {
    return row_;
}

int Position::col() const {
    return col_;
}

[[nodiscard]] bool Position::validation(const Position &pos) {
    if (int row = pos.row(); row < 0 || row >= board_rows) {
        return false;
    }
    if (int col = pos.col(); col < 0 || col >= board_cols) {
        return false;
    }
    return true;
}

bool operator==(const Position &lhs, const Position &rhs) {
    return lhs.row_ == rhs.row_ && lhs.col_ == rhs.col_;
}

} // namespace Chess::Logic
