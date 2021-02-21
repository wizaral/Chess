#pragma once
#include <cstdint>

namespace Chess {

class Position {
    int8_t m_row, m_col;

public:
    static const Position invalid;

    Position(int row, int col);

    int row() const;
    int col() const;

    static bool validation(Position pos);

    friend bool operator==(Position lhs, Position rhs);
    friend bool operator!=(Position lhs, Position rhs);
};

inline const Position Position::invalid{-1, -1};

} // namespace Chess
