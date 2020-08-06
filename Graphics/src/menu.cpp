#include "game.hpp"

void Game::menu() {
    bool exit = false;
    std::wstring path(L"stockfish.exe");

    board_.first.setPosition(0, 0);
    background_.first.setPosition(0, 0);
    std::array<int, 4> positions{1, 0, 0, 1};

    buttons_[0].first.setPosition(100, 100);
    buttons_[1].first.setPosition(450, 100);
    buttons_[2].first.setPosition(100, 400);
    buttons_[3].first.setPosition(450, 400);
    buttons_[4].first.setPosition(100, 550);

    std::array<std::unique_ptr<Chess::Player>, 2> players;

    while (window_.isOpen() && !exit) {
        sf::Event e;

        while (window_.pollEvent(e)) {
            if (Game::is_exit(e)) {
                window_.close();
            }

            if (e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left) {
                auto pos = sf::Mouse::getPosition(window_);

                if (buttons_[0].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[0] = 1;
                    positions[1] = 0;
                }
                if (buttons_[1].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[0] = 0;
                    positions[1] = 1;
                }
                if (buttons_[2].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[2] = 1;
                    positions[3] = 0;
                }
                if (buttons_[3].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[2] = 0;
                    positions[3] = 1;
                }
                if (buttons_[4].first.getGlobalBounds().contains(pos.x, pos.y - 30)) {
                    exit = true;
                }
            }

            if (e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space)) {
                exit = true;
            }
        }

        window_.draw(background_.first);
        for (auto &i : buttons_)
            window_.draw(i.first);

        for (int i = 0; i < 4; ++i) {
            if (positions[i] > 0) {
                rect_.first.setPosition(buttons_[i].first.getPosition() - sf::Vector2f{4, -4});
                window_.draw(rect_.first);
            }
        }
        window_.display();
    }

    if (window_.isOpen()) {
        if (positions[0]) {
            players[1] = std::make_unique<GraphicsBotPlayer>(Chess::FigureColor::Black, "Black bot", move_, log_, path, tile_size_, window_, dragg_pos_, dragging_);
        } else {
            players[1] = std::make_unique<RealPlayer>(Chess::FigureColor::Black, "Black player", move_, tile_size_, window_, dragg_pos_, dragging_);
        }

        if (positions[2]) {
            players[0] = std::make_unique<GraphicsBotPlayer>(Chess::FigureColor::White, "White bot", move_, log_, path, tile_size_, window_, dragg_pos_, dragging_);
        } else {
            players[0] = std::make_unique<RealPlayer>(Chess::FigureColor::White, "White player", move_, tile_size_, window_, dragg_pos_, dragging_);
        }

        logic_ = std::make_unique<Chess::Logic>(std::move(players));
        logic_->init_game(std::make_unique<Chess::ClassicFactory>());
        loop();
    }
}
