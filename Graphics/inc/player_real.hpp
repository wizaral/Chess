#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "chess.hpp"

class RealPlayer : public Chess::Player {
    const float half_tile_;
    Chess::Position &dragging_;
    sf::Vector2f &dragg_pos_;

    sf::RenderWindow &window_;
    std::optional<Chess::Move> &move_;

public:
    RealPlayer(Chess::FigureColor color, const std::string &name, float tile_size, Chess::Position &dragging, sf::Vector2f &dragg_pos, sf::RenderWindow &window, std::optional<Chess::Move> &move);
    Chess::Move get_next_move() override;
    Chess::FigureType promote_figure(const Chess::Position &pos) override;
};
