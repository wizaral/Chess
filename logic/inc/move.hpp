#pragma once
#include <cmath>

#include "position.hpp"

namespace Chess::Logic {

class Move {
    Position from_, to_;
public:
    Move(const Position &from, const Position &to);

    Position from() const;
    Position to() const;

    int rows() const;
    int cols() const;
};

} // namespace Chess::Logic
