#include "player.hpp"

namespace Chess {

Player::Player(FigureColor color)
: m_color(color) {}

FigureColor Player::color() const {
    return m_color;
}

} // namespace Chess
