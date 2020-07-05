#pragma once
#include <cmath>

#include "position.hpp"

namespace Chess::Logic {

class Move {
    Position from_, to_;
public:
    Move(const Position &from, const Position &to)
        : from_(from), to_(to) {}

    Position from() const {
        return from_;
    }

    Position to() const {
        return to_;
    }

    int rows() const {
        return abs(from_.row() - to_.row());
    }

    int cols() const {
        return abs(from_.col() - to_.col());
    }
};

} // namespace Chess::Logic
