#include "position.hpp"
#include "definitions.hpp"

namespace Chess {

Position::Position(int row, int col)
: m_row(row)
, m_col(col) {}

int Position::row() const {
    return m_row;
}

int Position::col() const {
    return m_col;
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
    return lhs.m_row == rhs.m_row && lhs.m_col == rhs.m_col;
}

bool operator!=(const Position &lhs, const Position &rhs) {
    return !(lhs == rhs);
}

} // namespace Chess
