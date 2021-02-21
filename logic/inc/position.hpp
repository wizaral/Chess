#pragma once
#include <cstdint>

namespace Chess {

class Position {
    int8_t m_row, m_col;

public:
    Position(int row, int col);

    int row() const;
    int col() const;

    static bool validation(Position pos);

    friend bool operator==(Position lhs, Position rhs);
    friend bool operator!=(Position lhs, Position rhs);
};

} // namespace Chess
