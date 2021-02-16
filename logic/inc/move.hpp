#pragma once
#include <cmath>

#include "position.hpp"

namespace Chess {

class Move {
    Position m_from, m_to;

public:
    Move(const Position &from, const Position &to);

    Position from() const;
    Position to() const;

    int rows() const;
    int cols() const;
};

} // namespace Chess
