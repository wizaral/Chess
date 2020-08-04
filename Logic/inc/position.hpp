#pragma once
#include <stdexcept>

namespace Chess {

class Position {
    int row_, col_;

public:
    Position(int row, int col);

    int row() const;
    int col() const;

    [[nodiscard]] static bool validation(const Position &pos);

    friend bool operator==(const Position &lhs, const Position &rhs);
};

} // namespace Chess
