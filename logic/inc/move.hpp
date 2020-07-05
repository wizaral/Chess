#pragma once
#include <utility>

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
};

} // namespace Chess::Logic
