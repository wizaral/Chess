#pragma once
#include <string>
#include "chess.hpp"

class PlayerBase : public Chess::Player {
protected:
    std::string m_name;

public:
    PlayerBase(Chess::FigureColor color, std::string name);
    const std::string &name() const;
};
