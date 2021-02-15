#include "player.hpp"

namespace Chess {

Player::Player(FigureColor color, const std::string &name)
: m_color(color)
, m_name(name) {}

FigureColor Player::color() const {
    return m_color;
}

const std::string &Player::name() const {
    return m_name;
}

} // namespace Chess
