#pragma once
#include <stdexcept>

namespace Chess::Logic {

class Position {
    int row_, col_;

public:
    Position(int row, int col);

    int row() const;
    int col() const;

    static void validation(const Position &pos);
};

} // namespace Chess::Logic
