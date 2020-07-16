#include <array>
#include <iostream>

#include "chess.hpp"
#include "c_input.hpp"
#include "c_render.hpp"

// void test(Chess::ChessGame& game) {
//     std::string colors[]{ "W", "B" };
//     std::string figures[]{ "P", "N", "B", "R", "Q", "K" };
//     const auto& b = game.b();

//     for (int i = 0; i < Chess::Logic::board_rows; ++i) {
//         for (int j = 0; j < Chess::Logic::board_cols; ++j) {
//             if (auto f = b.get_figure({ i, j }); f != nullptr) {
//                 std::cout << colors[static_cast<int>(f->color())] << ':' << figures[static_cast<int>(f->type())] << ' ';
//             }
//         }
//         std::cout << std::endl;
//     }
// }

int main() {
    // std::cout << players[0]->name() << ' ' << static_cast<int>(players[0]->color()) << ' ';
    // std::cout << players[1]->name() << ' ' << static_cast<int>(players[1]->color()) << ' ';
    // std::cout << std::endl;

    Chess::ChessGame game({
        std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::Light, "light"),
        std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::Dark, "dark"),
        }, std::make_unique<ConsoleRender>(), std::make_unique<ConsoleInput>());

    game.init_game();

    // test(game);
    game.loop();

}
