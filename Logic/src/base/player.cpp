#include "player.hpp"

namespace Chess::Logic {

Player::Player(FigureColor color, std::string name)
    : color_(color), name_(name) {}

FigureColor Player::color() const {
    return color_;
}

const std::string &Player::name() const {
    return name_;
}

} // namespace Chess::Logic
