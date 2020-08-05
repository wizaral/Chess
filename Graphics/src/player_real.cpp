#include "player_real.hpp"
#include "game.hpp"

RealPlayer::RealPlayer(Chess::FigureColor color, const std::string &name, float tile_size, Chess::Position &dragging, sf::RenderWindow &window, std::optional<Chess::Move> &move)
    : Chess::Player(color, name)
    , half_tile_(tile_size / 2)
    , dragging_(dragging)
    , window_(window)
    , move_(move) {}

Chess::Move RealPlayer::get_next_move() {
    sf::Event event;

    while (window_.isOpen() && window_.pollEvent(event)) {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed) {
            window_.close();
        } else if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window_);
            dragging_ = Game::transform(mpos);

            if (Chess::Position::validation(dragging_) == false) {
                dragging_ = Chess::Position{ -1, -1 };
            }
        } else if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window_);
            move_.emplace(dragging_, Game::transform(mpos));
            dragging_ = Chess::Position{-1, -1};
        }
    }
    return {{-1, -1}, {-1, -1}}; // I love OOP & OOD
}

#include <iostream>

Chess::FigureType RealPlayer::promote_figure(const Chess::Position &pos) {
    Chess::FigureType type = Chess::FigureType::Pawn;
    sf::Event event;

    while (window_.isOpen() && window_.waitEvent(event) && type == Chess::FigureType::Pawn) {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed) {
            window_.close();
        } else if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window_);
            std::cout << "mpos" << std::endl;

            if (auto ppos = Game::transform(mpos); ppos == pos) {
                auto [dx, dy] = Game::transform(ppos);
                int x = mpos.x - dx, y = mpos.y - dy;
                std::cout << ppos.row() << '.' << ppos.col() << "==" << pos.row() << '.' << pos.col() << std::endl;
                std::cout << x << ":" << dx << "-" << y << ":" << dy << std::endl;

                if (x < half_tile_ && y < half_tile_) {
                    return Chess::FigureType::Rook;
                } else if (x >= half_tile_ && y < half_tile_) {
                    return Chess::FigureType::Queen;
                } else if (x < half_tile_ && y >= half_tile_) {
                    return Chess::FigureType::Bishop;
                } else /* if (x >= half_tile_ && y >= half_tile_) */ {
                    return Chess::FigureType::Knight;
                }
            }
        }
    }
    return type;
}
