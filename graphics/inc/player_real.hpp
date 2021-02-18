#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "player_base.hpp"

class RealPlayer : public PlayerBase {
    std::optional<Chess::Move> &m_move;

    const float m_half_tile;
    sf::RenderWindow &m_window;
    sf::Vector2f &m_dragg_pos;
    Chess::Position &m_dragging;

public:
    RealPlayer(Chess::FigureColor color, const std::string &name, std::optional<Chess::Move> &move, float tile_size, sf::RenderWindow &window, sf::Vector2f &dragg_pos, Chess::Position &dragging);

    Chess::Move get_next_move() override;
    Chess::FigureType promote_figure(const Chess::Position &pos) override;
};
