#pragma once
#include <memory>
#include "board.hpp"
#include "figure.hpp"
#include "move.hpp"

namespace Chess::Logic {

struct Strategy {
    virtual void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) = 0;
};

}; // namespace Chess::Logic
