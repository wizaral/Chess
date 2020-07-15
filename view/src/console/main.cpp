#include <array>
#include <iostream>

#include "game.hpp"
#include "c_input.hpp"
#include "c_render.hpp"
using std::cin, std::cout, std::endl;

int main() {
    std::array<std::unique_ptr<Chess::Logic::Player>, 2> players{
        std::make_unique<Chess::Logic::Player>(Chess::Logic::Player(Chess::Logic::FigureColor::Light, "light")),
        std::make_unique<Chess::Logic::Player>(Chess::Logic::Player(Chess::Logic::FigureColor::Dark, "dark")),
    };
    Chess::ChessGame game(std::move(players), std::make_unique<Chess::Render>(ConsoleRender()), std::make_unique<Chess::DataInput>(ConsoleInput()));

    game.init_game();
    game.loop();

}
