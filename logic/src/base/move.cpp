#include "move.hpp"

namespace Chess::Logic {

Move::Move(const Position &from, const Position &to)
    : from_(from), to_(to) {}

Position Move::from() const {
    return from_;
}

Position Move::to() const {
    return to_;
}

int Move::rows() const {
    return abs(from_.row() - to_.row());
}

int Move::cols() const {
    return abs(from_.col() - to_.col());
}

} // namespace Chess::Logic
