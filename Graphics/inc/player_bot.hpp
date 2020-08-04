#pragma once
#include <SFML/Graphics.hpp>
#include "chess.hpp"

class BotPlayer : public Chess::Logic::Player {
public:
    Chess::Logic::Move get_next_move() override;
    Chess::Logic::FigureType promote_figure(const Chess::Logic::Position& pos) override;
};
