#include "g_player.hpp"
#include "g_render.hpp"

RealPlayer::RealPlayer(Chess::Logic::FigureColor color, std::string name, sf::RenderWindow &window)
    : Chess::Logic::Player(color, name), window_(window) {}

Chess::Logic::Move RealPlayer::get_next_move() {
    Chess::Logic::Move move{{-1, -1}, {-1, -1}};
    int index = 0;
    sf::Event event;

    while (window_.isOpen() && index < 2 && window_.waitEvent(event)) {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed) {
            window_.close();
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window_);
            if (index == 0) {
                move = {GraphicsRender::transform(mpos), move.to()};
            } else {
                move = {move.from(), GraphicsRender::transform(mpos)};
            }
            ++index;
        }
    }
    return move;
}

Chess::Logic::FigureType RealPlayer::promote_figure(const Chess::Logic::Position &pos) {
    Chess::Logic::FigureType type = Chess::Logic::FigureType::Pawn;
    sf::Event event;

    while (window_.isOpen() && type == Chess::Logic::FigureType::Pawn && window_.waitEvent(event)) {
        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed) {
            window_.close();
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window_);

            if (auto ppos = GraphicsRender::transform(mpos); ppos == pos) {
                auto [dx, dy] = GraphicsRender::transform(ppos);
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
