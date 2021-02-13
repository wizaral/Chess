#include "game.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chess", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition({(static_cast<int>(sf::VideoMode::getDesktopMode().width) - window_width) / 2, 0});
    window.setFramerateLimit(60);

    Game game(window);

    try {
        game.menu();
    } catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
