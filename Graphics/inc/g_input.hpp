#pragma once
#include <SFML/Graphics.hpp>
#include "input.hpp"

class GraphicsInput : public Chess::DataInput {
public:
    Chess::Logic::Move get_next_move(Chess::Logic::Player* player) override;
    Chess::Logic::FigureType promote_figure(Chess::Logic::Player* player, const Chess::Logic::Position &pos) override;
};
