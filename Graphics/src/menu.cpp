#include "game.hpp"

void Game::menu() {
    std::wstring path(L"stockfish.exe");

    while (window_.isOpen() && !Chess::is_endgame(state_)) {
        logic_ = std::make_unique<Chess::Logic>(std::array<std::unique_ptr<Chess::Player>, 2>{
            // std::make_unique<RealPlayer>(Chess::FigureColor::White, "White virgin", tile_size_, dragging_, dragg_pos_, window_, move_),
            // std::make_unique<RealPlayer>(Chess::FigureColor::Black, "Black virgin", tile_size_, dragging_, dragg_pos_, window_, move_),
            std::make_unique<BotPlayer>(Chess::FigureColor::White, "White Chad", tile_size_, dragging_, dragg_pos_, log_, window_, move_, path),
            std::make_unique<BotPlayer>(Chess::FigureColor::Black, "Black Chad", tile_size_, dragging_, dragg_pos_, log_, window_, move_, path),
        });

        logic_->init_game(std::make_unique<Chess::ClassicFactory>());
        loop();
    }
}
