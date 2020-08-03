#include "g_input.hpp"
#include <iostream>

GraphicsInput::GraphicsInput(sf::RenderWindow &window) : window_(window) {}

Chess::Logic::Move GraphicsInput::get_next_move(Chess::Logic::Player *player) {
    Chess::Logic::Move move{{-1, -1}, {-1, -1}};
    int index = 0;

    while (window_.isOpen()) {
        sf::Event event;

        while (index < 2 && window_.waitEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window_.close();
            }
            if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
                sf::Vector2i pos = sf::Mouse::getPosition(window_);

                if (index == 0) {
                    move = {transform(pos), move.to()};
                } else {
                    move = {move.from(), transform(pos)};
                }
                ++index;
            }
        }
        return move;
    }
    return move;
}

Chess::Logic::FigureType GraphicsInput::promote_figure(Chess::Logic::Player *player, Chess::Logic::Position pos) {
    //
    return Chess::Logic::FigureType::Queen;
}

Chess::Logic::Position GraphicsInput::transform(const sf::Vector2i &pos) {
    Chess::Logic::Position p{7 - ((pos.y - static_cast<int>(offset)) / static_cast<int>(offset)), pos.x / static_cast<int>(offset)};
    std::cout << p.row() << ":" << p.col() << std::endl;
    return p;
}
