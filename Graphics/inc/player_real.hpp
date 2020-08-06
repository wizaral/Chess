#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "chess.hpp"

class RealPlayer : public Chess::Player {
    std::optional<Chess::Move> &move_;

    const float half_tile_;
    sf::RenderWindow &window_;
    sf::Vector2f &dragg_pos_;
    Chess::Position &dragging_;

public:
    RealPlayer(Chess::FigureColor color, const std::string &name, std::optional<Chess::Move> &move, float tile_size, sf::RenderWindow &window, sf::Vector2f &dragg_pos, Chess::Position &dragging);

    Chess::Move get_next_move() override;
    Chess::FigureType promote_figure(const Chess::Position &pos) override;
};
