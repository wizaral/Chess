#include "player_base.hpp"

PlayerBase::PlayerBase(Chess::FigureColor color, std::string name)
: Chess::Player(color)
, m_name(name) {}

const std::string &PlayerBase::name() const {
    return m_name;
}
