#include "game.hpp"

RealPlayer::RealPlayer(Chess::FigureColor color,
                       const std::string& name,
                       std::optional<Chess::Move>& move,
                       float tile_size,
                       sf::RenderWindow& window,
                       sf::Vector2f& dragg_pos,
                       Chess::Position& dragging)
: PlayerBase(color, name)
, m_move(move)
, m_half_tile(tile_size / 2)
, m_window(window)
, m_dragg_pos(dragg_pos)
, m_dragging(dragging) {}

Chess::Move RealPlayer::get_next_move() {
    sf::Event event;

    while (m_window.isOpen() && m_window.pollEvent(event)) {
        if (Game::is_exit(event)) {
            m_window.close();
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(m_window);
            m_dragging = Game::transform(mpos);

            if (Chess::Position::validation(m_dragging) == false) {
                m_dragging = Chess::Position{-1, -1};
            }
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(m_window);
            m_move.emplace(m_dragging, Game::transform(mpos));
            m_dragging = Chess::Position{-1, -1};
        }
        if (!(m_dragging == Chess::Position{-1, -1})) {
            m_dragg_pos = sf::Vector2f{sf::Mouse::getPosition(m_window)} - sf::Vector2f{m_half_tile, m_half_tile};
        }
    }
    return {{-1, -1}, {-1, -1}}; // I love OOP & OOD
}

Chess::FigureType RealPlayer::promote_figure(const Chess::Position &pos) {
    Chess::FigureType type = Chess::FigureType::Pawn;
    sf::Event event;

    while (m_window.isOpen() && m_window.waitEvent(event) && type == Chess::FigureType::Pawn) {
        if (Game::is_exit(event)) {
            m_window.close();
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(m_window);

            if (auto ppos = Game::transform(mpos); ppos == pos) {
                auto [dx, dy] = Game::transform(ppos);
                int x = mpos.x - dx, y = mpos.y - dy;

                if (x < m_half_tile && y < m_half_tile) {
                    return Chess::FigureType::Rook;
                } else if (x >= m_half_tile && y < m_half_tile) {
                    return Chess::FigureType::Queen;
                } else if (x < m_half_tile && y >= m_half_tile) {
                    return Chess::FigureType::Bishop;
                } else /* if (x >= m_half_tile && y >= m_half_tile) */ {
                    return Chess::FigureType::Knight;
                }
            }
        }
    }
    return type;
}
