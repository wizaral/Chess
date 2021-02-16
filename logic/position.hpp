#pragma once
#include <stdexcept>

namespace Chess {

class Position {
    int8_t m_row, m_col;

public:
    Position(int row, int col);

    int row() const;
    int col() const;

    [[nodiscard]] static bool validation(const Position &pos);

    friend bool operator==(const Position &lhs, const Position &rhs);
    friend bool operator!=(const Position &lhs, const Position &rhs);
};

} // namespace Chess
