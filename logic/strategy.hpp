#pragma once
#include <vector>

#include "board.hpp"
#include "figure.hpp"
#include "gamestate.hpp"
#include "move.hpp"
#include "observer.hpp"

namespace Chess {

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual GameState validate_move(const Board &board, const Move &move) const = 0;
    virtual void move_update(const Move &move) = 0;

    virtual void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const = 0;
    virtual void update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const = 0;
};

} // namespace Chess
