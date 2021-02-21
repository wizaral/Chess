#pragma once
#include <cmath>

#include "position.hpp"

namespace Chess {

class Move {
    Position m_from, m_to;

public:
    Move(Position from, Position to);

    Position from() const;
    Position to() const;

    int rows() const;
    int cols() const;
};

} // namespace Chess
