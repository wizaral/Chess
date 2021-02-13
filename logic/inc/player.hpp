#pragma once
#include <string>

#include "figure.hpp"
#include "move.hpp"

namespace Chess {

class Player {
    FigureColor color_;
    std::string name_;

public:
    Player(FigureColor color, const std::string &name);
    virtual ~Player() = default;

    FigureColor color() const;
    const std::string &name() const;

    virtual Move get_next_move() = 0;
    virtual FigureType promote_figure(const Chess::Position &pos) = 0;
};

} // namespace Chess
