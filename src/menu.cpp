#include "game.hpp"

void Game::print_menu(std::array<int, 4> &positions) {
    bool exit = false;

    while (m_window.isOpen() && !exit) {
        sf::Event e;

        while (m_window.pollEvent(e)) {
            if (Game::is_exit(e)) {
                m_window.close();
            }

            if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
                auto pos = sf::Mouse::getPosition(m_window);

                if (m_buttons[0].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[0] = 1;
                    positions[1] = 0;
                } else if (m_buttons[1].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[0] = 0;
                    positions[1] = 1;
                } else if (m_buttons[2].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[2] = 1;
                    positions[3] = 0;
                } else if (m_buttons[3].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    positions[2] = 0;
                    positions[3] = 1;
                } else if (m_buttons[4].first.getGlobalBounds().contains(pos.x, pos.y)) {
                    exit = true;
                }
            }

            if (e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space)) {
                exit = true;
            }
        }

        m_window.draw(m_background.first);
        for (auto &i : m_buttons)
            m_window.draw(i.first);

        for (int i = 0; i < 4; ++i) {
            if (positions[i] > 0) {
                m_rect.first.setPosition(m_buttons[i].first.getPosition() - sf::Vector2f{4, -4});
                m_window.draw(m_rect.first);
            }
        }
        m_window.display();
    }
}

void Game::menu() {
#if defined(_WIN64) || defined(_WIN32)
    std::string path("stockfish.exe");
#elif defined(__APPLE__) || defined(__linux__)
    std::string path("PUT_CORRECT_PATH_HERE");
#else
#error unsupported OS
#endif

    std::array<int, 4> positions{1, 0, 1, 0};
    std::array<std::unique_ptr<Chess::Player>, Chess::players_amount> players;

    print_menu(positions);

    if (m_window.isOpen()) {
        if (positions[0]) {
            players[1] = std::make_unique<GraphicsBotPlayer>(Chess::FigureColor::Black, "Black bot", m_move, m_log, path, s_tile_size, m_window, m_dragg_pos, m_dragging);
        } else {
            players[1] = std::make_unique<RealPlayer>(Chess::FigureColor::Black, "Black player", m_move, s_tile_size, m_window, m_dragg_pos, m_dragging);
        }

        if (positions[2]) {
            players[0] = std::make_unique<GraphicsBotPlayer>(Chess::FigureColor::White, "White bot", m_move, m_log, path, s_tile_size, m_window, m_dragg_pos, m_dragging);
        } else {
            players[0] = std::make_unique<RealPlayer>(Chess::FigureColor::White, "White player", m_move, s_tile_size, m_window, m_dragg_pos, m_dragging);
        }

        m_logic = std::make_unique<Chess::Logic>(std::move(players));
        m_logic->init_game(std::make_unique<Chess::ClassicFactory>());
        loop();
    }
}
