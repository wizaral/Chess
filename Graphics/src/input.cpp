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
                int x = mpos.x - ppos.col() * offset;
                int y = mpos.y - (8 - ppos.row()) * offset;

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
    Chess::Logic::Position p{7 - ((pos.y - static_cast<int>(offset)) / static_cast<int>(offset)), pos.x / static_cast<int>(offset)};
    std::cout << p.row() << ":" << p.col() << std::endl;
    return p;
}
