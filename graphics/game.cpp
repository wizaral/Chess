#include "game.hpp"

Game::Game(sf::RenderWindow &window)
    : m_window(window) {
    load();
}

void Game::load() {
    load(m_rect, GAME_RESOURCES_DIR"rect.png");
    load(m_board, GAME_RESOURCES_DIR"board.png");
    load(m_background, GAME_RESOURCES_DIR"bg.jpg");

    load(m_promotions[0], GAME_RESOURCES_DIR"wPp.png");
    load(m_promotions[1], GAME_RESOURCES_DIR"bPp.png");

    load(m_buttons[0], GAME_RESOURCES_DIR"pcb.png");
    load(m_buttons[1], GAME_RESOURCES_DIR"psb.png");
    load(m_buttons[2], GAME_RESOURCES_DIR"pcw.png");
    load(m_buttons[3], GAME_RESOURCES_DIR"psw.png");
    load(m_buttons[4], GAME_RESOURCES_DIR"play.png");

    std::string path{GAME_RESOURCES_DIR"__.png"};
    const std::array<char, 6> figures{'P', 'N', 'B', 'R', 'Q', 'K'};
    const std::array<char, 2> colors{'w', 'b'};

    auto color_index = path.find_first_of('_');
    auto type_index = path.find_last_of('_');

    for (int i = 0; i < 2; ++i) {
        path[color_index] = colors[i];
        for (int j = 0; j < 6; ++j) {
            path[type_index] = figures[j];
            load(m_figures[i][j], path);
        }
    }
    m_font.loadFromFile(GAME_RESOURCES_DIR"sansation.ttf");

    m_board.first.setPosition(0, 0);
    m_background.first.setPosition(0, 0);

    m_buttons[0].first.setPosition(100, 100);
    m_buttons[1].first.setPosition(450, 100);
    m_buttons[2].first.setPosition(100, 400);
    m_buttons[3].first.setPosition(450, 400);
    m_buttons[4].first.setPosition(130, 700);
}

void Game::load(Sprite &sprt, const std::string &path) {
    auto &[sprite, texture] = sprt;
    texture.loadFromFile(path);
    sprite.setTexture(texture);
}

void Game::loop() {
    while (m_window.isOpen() && !Chess::is_endgame(m_state)) {
        reprint();

        if (m_state == Chess::GameState::PawnPromotion) {
            pawn_promotion();
        } else {
            m_logic->player()->get_next_move();
        }

        if (m_move.has_value()) {
            m_state = m_logic->logic(m_move.value());

            if (Chess::is_move(m_state)) {
                log();
            }

            if (is_castling(m_state)) {
                castling();
            }

            if (m_state == Chess::GameState::PawnPromotion) {
                m_promoting = m_move->to();
            }
            reprint();
            m_move.reset();
        }

        print_state();
        m_window.display();
    }

    after_game();
}

void Game::pawn_promotion() {
    m_state = m_logic->promote_pawn(m_logic->player()->promote_figure(m_promoting));

    if (m_state != Chess::GameState::PawnPromotion) {
        char ftype = '\0';

        switch (auto type = m_logic->board().get_figure(m_promoting)->type(); type) {
        case Chess::FigureType::Bishop:
            ftype = 'b';
            break;
        case Chess::FigureType::Knight:
            ftype = 'n';
            break;
        case Chess::FigureType::Rook:
            ftype = 'r';
            break;
        default:
            ftype = 'q';
        }

        m_log << ftype;
        m_promoting = Chess::Position{-1, -1};
    }
}

void Game::castling() {
    int row = m_move->from().row();

    if (int distance = m_move->from().col() - m_move->to().col(); distance < 0) {
        m_move = Chess::Move{{row, 7}, {row, 5}};
    } else /* if (distance > 0) */ {
        m_move = Chess::Move{{row, 0}, {row, 3}};
    }
    print_castling();
}

void Game::log() {
    m_log << ' ';
    m_log << static_cast<char>(m_move->from().col() + 97) << static_cast<char>(m_move->from().row() + 49);
    m_log << static_cast<char>(m_move->to().col() + 97) << static_cast<char>(m_move->to().row() + 49);
}

bool Game::is_exit(const sf::Event &e) {
    return e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape);
}

Chess::Position Game::transform(const sf::Vector2i &pos) {
    int off = static_cast<int>(s_tile_size);
    return {7 - pos.y / off, pos.x / off};
}

sf::Vector2f Game::transform(const Chess::Position &pos) {
    return {pos.col() * s_tile_size, (7 - pos.row()) * s_tile_size};
}
