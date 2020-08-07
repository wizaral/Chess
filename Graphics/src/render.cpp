#include "game.hpp"

void Game::reprint() {
    window_.clear();
    print_board();
    print_next_step();
}

void Game::print_castling() {
    sf::Vector2f dest{Game::transform(move_->to())};
    dragging_ = move_->to();
    dragg_pos_ = Game::transform(move_->from());
    auto delta = (dest - dragg_pos_) / 25.f;

    while (window_.isOpen() && dragg_pos_ != dest) {
        sf::Event event;

        while (window_.pollEvent(event)) {
            if (Game::is_exit(event)) {
                window_.close();
            }
        }

        dragg_pos_ += delta;
        reprint();
        print_state();
        window_.display();
    }
    dragging_ = Chess::Position{-1, -1};
}

void Game::after_game() {
    if (window_.isOpen()) {
        sf::Text text("Press any button to exit", font_);
        text.setFillColor(sf::Color(255, 255, 255));
        text.setPosition(10.f, 860.f);

        window_.clear();
        print_board();
        print_endgame();
        window_.draw(text);
        window_.display();

        sf::Event event;
        while (window_.isOpen() && window_.waitEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
                window_.close();
            }
        }
    }
}

void Game::print_board() {
    window_.draw(board_.first);
    const Chess::Board &board = logic_->board();

    for (int row = Chess::board_rows - 1; row > -1; --row) {
        for (int col = 0; col < Chess::board_cols; ++col) {
            if (auto f = board.get_figure({row, col}); f != nullptr && dragging_ != Chess::Position{row, col}) {
                if (Chess::Position p{row, col}; p == promoting_) {
                    auto &promotion = promotions_[static_cast<int>(f->color())].first;
                    promotion.setPosition(transform(p));
                    window_.draw(promotion);
                } else {
                    auto &figure = figures_[static_cast<int>(f->color())][static_cast<int>(f->type())].first;
                    figure.setPosition(transform(p));
                    window_.draw(figure);
                }
            }
        }
    }

    if (dragging_ != Chess::Position{-1, -1}) {
        if (auto f = logic_->board().get_figure(dragging_); f != nullptr) {
            auto &figure = figures_[static_cast<int>(f->color())][static_cast<int>(f->type())].first;
            figure.setPosition(dragg_pos_);
            window_.draw(figure);
        }
    }
}

void Game::print_next_step() {
    std::ostringstream oss;
    oss << logic_->player()->name() << "'s turn";

    sf::Text text(oss.str(), font_);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 810.f);
    window_.draw(text);
}

void Game::print_state() {
    std::ostringstream oss;
    if (Chess::is_error(state_)) {
        oss << "Error: ";
    }
    oss << states[static_cast<int>(state_)];

    sf::Text text(oss.str(), font_);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 860.f);
    window_.draw(text);
}

void Game::print_endgame() {
    std::ostringstream oss;
    oss << "Game over: ";

    if (state_ == Chess::GameState::CheckMate) {
        oss << "Player " << logic_->player()->name() << " won!";
    } else {
        oss << states[static_cast<int>(state_)];
    }

    sf::Text text(oss.str(), font_);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 810.f);
    window_.draw(text);
}
