#include <array>
#include <iostream>

#include "chess.hpp"
#include "c_input.hpp"
#include "c_render.hpp"

int main() {
    Chess::ChessGame game({
        std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::White, "white"),
        std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::Black, "black"),
        }, std::make_unique<Chess::ClassicFactory>(), std::make_unique<ConsoleRender>(), std::make_unique<ConsoleInput>());

    game.init_game();
    game.loop();
}
