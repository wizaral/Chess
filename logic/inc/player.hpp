#pragma once
#include <string>

#include "figure.hpp"

namespace Chess {

class Player {
    FigureColor m_color;
    std::string m_name;

public:
    Player(FigureColor color, const std::string &name);
    virtual ~Player() = default;

    FigureColor color() const;
    const std::string &name() const;

    virtual Move get_next_move() = 0;
    virtual FigureType promote_figure(const Chess::Position &pos) = 0;
};

} // namespace Chess
