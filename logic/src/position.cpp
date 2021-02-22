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

bool Position::validation(Position pos) {
    int row = pos.row(), col = pos.col();
    return row > -1 && row < board_rows && col > -1 && col < board_cols;
}

bool operator==(Position lhs, Position rhs) {
    return lhs.m_row == rhs.m_row && lhs.m_col == rhs.m_col;
}

bool operator!=(Position lhs, Position rhs) {
    return !operator==(lhs, rhs);
}

} // namespace Chess
