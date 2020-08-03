#include "g_input.hpp"
#include "g_render.hpp"
#include "chess.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chess", sf::Style::Titlebar | sf::Style::Close);
    const auto& mode = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i{(static_cast<int>(mode.width) - window_width) / 2, 0});

    Chess::ChessGame game({
        std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::White, "white"),
        std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::Black, "black"),
        },
        std::make_unique<Chess::ClassicFactory>(),
        std::make_unique<GraphicsRender>(window),
        std::make_unique<GraphicsInput>(window),
        [&window]() -> bool {
            return window.isOpen();
        });

    game.init_game();
    game.loop();
}
