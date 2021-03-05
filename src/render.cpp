#include "game.hpp"

void Game::reprint() {
    m_window.clear();
    print_board();
    print_next_step();
}

void Game::print_castling() {
    sf::Vector2f dest{Game::transform(m_move->to())};
    m_dragging = m_move->to();
    m_dragg_pos = Game::transform(m_move->from());
    auto delta = (dest - m_dragg_pos) / 25.f;

    while (m_window.isOpen() && m_dragg_pos != dest) {
        sf::Event event;

        while (m_window.pollEvent(event)) {
            if (Game::is_exit(event)) {
                m_window.close();
            }
        }

        m_dragg_pos += delta; // m_dragg_pos = dest;
        reprint();
        print_state();
        m_window.display();
    }
    m_dragging = Chess::Position::invalid;
}

void Game::after_game() {
    if (m_window.isOpen()) {
        sf::Text text("Press any button to exit", m_font);
        text.setFillColor(sf::Color(255, 255, 255));
        text.setPosition(10.f, 860.f);

        m_window.clear();
        print_board();
        print_endgame();
        m_window.draw(text);
        m_window.display();

        sf::Event event;
        while (m_window.isOpen() && m_window.waitEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
                m_window.close();
            }
        }
    }
}

void Game::print_board() {
    m_window.draw(m_board.first);
    const Chess::Board &board = m_logic->board();

    for (int row = Chess::board_rows - 1; row > -1; --row) {
        for (int col = 0; col < Chess::board_cols; ++col) {
            if (auto f = board.get_figure({row, col}); f != nullptr && m_dragging != Chess::Position{row, col}) {
                if (Chess::Position p{row, col}; p == m_promoting) {
                    auto &promotion = m_promotions[static_cast<int>(f->color())].first;
                    promotion.setPosition(transform(p));
                    m_window.draw(promotion);
                } else {
                    auto &figure = m_figures[static_cast<int>(f->color())][static_cast<int>(f->type())].first;
                    figure.setPosition(transform(p));
                    m_window.draw(figure);
                }
            }
        }
    }

    if (m_dragging != Chess::Position::invalid) {
        if (auto f = m_logic->board().get_figure(m_dragging); f != nullptr) {
            auto &figure = m_figures[static_cast<int>(f->color())][static_cast<int>(f->type())].first;
            figure.setPosition(m_dragg_pos);
            m_window.draw(figure);
        }
    }
}

void Game::print_next_step() {
    std::ostringstream oss;
    oss << static_cast<PlayerBase *>(m_logic->player())->name() << "'s turn";

    sf::Text text(oss.str(), m_font);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 810.f);
    m_window.draw(text);
}

void Game::print_state() {
    std::ostringstream oss;
    if (Chess::is_error(m_state)) {
        oss << "Error: ";
    }
    oss << s_states[static_cast<int>(m_state)];

    sf::Text text(oss.str(), m_font);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 860.f);
    m_window.draw(text);
}

void Game::print_endgame() {
    std::ostringstream oss;
    oss << "Game over: ";

    if (m_state == Chess::GameState::CheckMate) {
        oss << "Player " << static_cast<PlayerBase *>(m_logic->player())->name() << " won!";
    } else {
        oss << s_states[static_cast<int>(m_state)];
    }

    sf::Text text(oss.str(), m_font);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 810.f);
    m_window.draw(text);
}
