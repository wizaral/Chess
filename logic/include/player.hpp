#pragma once
#include "figure.hpp"

namespace Chess {

class Player {
protected:
    FigureColor m_color;

public:
    Player(FigureColor color);
    virtual ~Player() = default;

    FigureColor color() const;

    virtual Move get_next_move() = 0;
    virtual FigureType promote_figure(Position pos) = 0;
};

} // namespace Chess
