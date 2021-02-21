#include "move.hpp"

namespace Chess {

Move::Move(Position from, Position to)
: m_from(from)
, m_to(to) {}

Position Move::from() const {
    return m_from;
}

Position Move::to() const {
    return m_to;
}

int Move::rows() const {
    return std::abs(m_from.row() - m_to.row());
}

int Move::cols() const {
    return std::abs(m_from.col() - m_to.col());
}

} // namespace Chess
