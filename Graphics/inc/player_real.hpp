#pragma once
#include <SFML/Graphics.hpp>
#include "chess.hpp"

class RealPlayer : public Chess::Logic::Player {
    sf::RenderWindow &window_;
public:
    RealPlayer(Chess::Logic::FigureColor color, std::string name, sf::RenderWindow &window);
    Chess::Logic::Move get_next_move() override;
    Chess::Logic::FigureType promote_figure(const Chess::Logic::Position &pos) override;
};
