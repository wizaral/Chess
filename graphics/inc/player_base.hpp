#pragma once
#include <string>
#include "chess.hpp"

class PlayerBase : public Chess::Player {
protected:
    std::string m_name;

public:
    PlayerBase(Chess::FigureColor color, std::string name);
    const std::string &name() const;

    virtual Chess::Move get_next_move() = 0;
    virtual Chess::FigureType promote_figure(const Chess::Position &pos) = 0;
};
