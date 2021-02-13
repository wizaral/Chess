#include <array>
#include <iostream>

#include "chess.hpp"
#include "c_input.hpp"
#include "c_render.hpp"

int main() {
    Chess::Logic game({
        std::make_unique<Chess::Player>(Chess::FigureColor::White, "white"),
        std::make_unique<Chess::Player>(Chess::FigureColor::Black, "black"),
        }, std::make_unique<Chess::ClassicFactory>(), std::make_unique<ConsoleRender>(), std::make_unique<ConsoleInput>());

    game.init_game();
    game.loop();
}
