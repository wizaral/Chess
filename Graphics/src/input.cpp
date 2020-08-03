#include "g_input.hpp"
#include <iostream>

GraphicsInput::GraphicsInput(sf::RenderWindow &window) : window_(window) {}

Chess::Logic::Move GraphicsInput::get_next_move(Chess::Logic::Player *player) {
    Chess::Logic::Move move{{-1, -1}, {-1, -1}};
    int index = 0;
    sf::Event event;

    while (window_.isOpen() && index < 2 && window_.waitEvent(event)) {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed) {
            window_.close();
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window_);
            move = index == 0 ? Chess::Logic::Move{transform(mpos), move.to()} : Chess::Logic::Move{move.from(), transform(mpos)};
            ++index;
        }
    }
    return move;
}

Chess::Logic::FigureType GraphicsInput::promote_figure(Chess::Logic::Player *player, const Chess::Logic::Position &pos) {
    Chess::Logic::FigureType type = Chess::Logic::FigureType::Pawn;
    sf::Event event;

    while (window_.isOpen() && type == Chess::Logic::FigureType::Pawn && window_.waitEvent(event)) {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed) {
            window_.close();
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window_);

            if (auto ppos = transform(mpos); ppos == pos) {
                auto [dx, dy] = transform(ppos);
                int x = mpos.x - dx, y = mpos.y - dy;

                if (x < 50 && y < 50) {
                    type = Chess::Logic::FigureType::Rook;
                }
                else if (x > 49 && y < 50) {
                    type = Chess::Logic::FigureType::Queen;
                }
                else if (x < 50 && y > 49) {
                    type = Chess::Logic::FigureType::Bishop;
                }
                else /* if (x > 49 && y > 49) */ {
                    type = Chess::Logic::FigureType::Knight;
                }
            }
        }
    }
    return type;
}

Chess::Logic::Position GraphicsInput::transform(const sf::Vector2i &pos) {
    int off = static_cast<int>(offset);
    Chess::Logic::Position p{8 - pos.y / off, pos.x / off};
    std::cout << "Pos " << p.row() << ":" << p.col() << std::endl;
    return p;
}

sf::Vector2f GraphicsInput::transform(const Chess::Logic::Position& pos) {
    sf::Vector2f p{ pos.col() * offset, (8 - pos.row()) * offset };
    std::cout << "Vec " << p.x << ":" << p.y << std::endl;
    return p;
}
