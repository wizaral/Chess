#pragma once
#include "figure.hpp"
#include "move.hpp"
#include "player.hpp"

namespace Chess {

class DataInput {
public:
    virtual ~DataInput() = default;
    virtual Logic::Move get_next_move(Logic::Player *player) = 0;
    virtual Logic::FigureType promote_figure(Logic::Player *player, const Logic::Position &pos) = 0;
};

} // namespace Chess
