#pragma once
#include <SFML/Graphics.hpp>
#include "input.hpp"

class GraphicsInput : public Chess::DataInput {
    sf::RenderWindow &window_;

    inline const static float offset = 100.0f;
public:
    GraphicsInput(sf::RenderWindow &window);
    Chess::Logic::Move get_next_move(Chess::Logic::Player* player) override;
    Chess::Logic::FigureType promote_figure(Chess::Logic::Player* player, Chess::Logic::Position pos) override;
private:
   static Chess::Logic::Position transform(const sf::Vector2i &pos);
};
