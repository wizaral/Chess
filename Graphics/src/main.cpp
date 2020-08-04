#include "g_input.hpp"
#include "g_render.hpp"
#include "g_player.hpp"
#include "chess.hpp"

void after_game(sf::RenderWindow &window) {
    for (sf::Event event; window.isOpen() && window.waitEvent(event);) {
        if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
            window.close();
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chess", sf::Style::Titlebar | sf::Style::Close);
    const auto& mode = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i{(static_cast<int>(mode.width) - window_width) / 2, 0});

    Chess::ChessGame game({
        std::make_unique<RealPlayer>(Chess::Logic::FigureColor::White, "white", window),
        std::make_unique<RealPlayer>(Chess::Logic::FigureColor::Black, "black", window),
        },
        std::make_unique<Chess::ClassicFactory>(),
        std::make_unique<GraphicsRender>(window),
        std::make_unique<GraphicsInput>(),
        [&window]() -> bool {
            return window.isOpen();
        });

    game.init_game();
    game.loop();
    after_game(window);
}
