#include "game.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition({(static_cast<int>(sf::VideoMode::getDesktopMode().width) - window_width) / 2, 0});
    window.setFramerateLimit(60);

    Game game(window);
    game.menu();
}
