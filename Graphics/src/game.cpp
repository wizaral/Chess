#include "game.hpp"
#include <sstream>

Game::Game(sf::RenderWindow &window) : window_(window) {
    load();
}

void Game::load() {
    load(board_, "res" DIR_SEP "board.png");
    board_.first.setPosition(0, 0);

    load(promotions_[0], "res" DIR_SEP "wPp.png");
    load(promotions_[1], "res" DIR_SEP "bPp.png");

    std::string path{"res" DIR_SEP "__.png"};
    const std::array<char, 6> figures{'P', 'N', 'B', 'R', 'Q', 'K'};
    const std::array<char, 2> colors{'w', 'b'};

    for (int i = 0; i < 2; ++i) {
        path[4] = colors[i];
        for (int j = 0; j < 6; ++j) {
            path[5] = figures[j];
            load(figures_[i][j], path);
        }
    }
    font_.loadFromFile("res" DIR_SEP "sansation.ttf");
}

void Game::load(Sprite &sprt, const std::string &path) {
    auto &[sprite, texture] = sprt;
    texture.loadFromFile(path);
    sprite.setTexture(texture);
}

void Game::menu() {
    logic_ = std::make_unique<Chess::Logic>(std::array<std::unique_ptr<Chess::Player>, 2>{
        std::make_unique<RealPlayer>(Chess::FigureColor::White, "white", tile_size_, dragging_, window_, move_),
        std::make_unique<RealPlayer>(Chess::FigureColor::Black, "black", tile_size_, dragging_, window_, move_)
    });

    logic_->init_game(std::make_unique<Chess::ClassicFactory>());
    loop();
}

void Game::loop() {
    while (window_.isOpen() && !Chess::is_endgame(state_)) {
        window_.clear();
        print_board();

        print_next_step();
        if (state_ == Chess::GameState::PawnPromotion) {
            pawn_promotion();
        } else {
            logic_->player()->get_next_move();
        }

        if (move_.has_value()) {
            state_ = logic_->logic(move_.value());
            // log(state_);
            if (state_ == Chess::GameState::PawnPromotion) {
                promoting_ = move_->to();
                window_.clear();
                print_board();
            }
            move_.reset();
        }

        print_state();
        window_.display();
    }

    after_game();
}

void Game::pawn_promotion() {
    state_ = logic_->promote_pawn(logic_->player()->promote_figure(promoting_));

    if (state_ != Chess::GameState::PawnPromotion) {
        promoting_ = Chess::Position{ -1, -1 };
    }
}

void Game::after_game() {
    sf::Text text("Press any button to exit", font_);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(410.f, 810.f);

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

void Game::print_board() {
    window_.draw(board_.first);
    const Chess::Board &board = logic_->board();

    for (int row = Chess::board_rows - 1; row > -1; --row) {
        for (int col = 0; col < Chess::board_cols; ++col) {
            if (auto f = board.get_figure({row, col}); f != nullptr && !(dragging_ == Chess::Position{row, col})) {
                if (Chess::Position p{row, col}; p == promoting_) {
                    auto& promotion = promotions_[static_cast<int>(f->color())].first;
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

    if (!(dragging_ == Chess::Position{-1, -1})) {
        sf::Vector2i mpos = sf::Mouse::getPosition(window_) - sf::Vector2i{static_cast<int>(tile_size_) / 2, static_cast<int>(tile_size_) / 2};
        if (auto f = logic_->board().get_figure(dragging_); f != nullptr) {
            auto& figure = figures_[static_cast<int>(f->color())][static_cast<int>(f->type())].first;
            figure.setPosition(sf::Vector2f(mpos));
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
    }
    else {
        oss << states[static_cast<int>(state_)];
    }

    sf::Text text(oss.str(), font_);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 810.f);
    window_.draw(text);
}

Chess::Position Game::transform(const sf::Vector2i& pos) {
    int off = static_cast<int>(tile_size_);
    return {7 - pos.y / off, pos.x / off};
}

sf::Vector2f Game::transform(const Chess::Position& pos) {
    return {pos.col() * tile_size_, (7 - pos.row()) * tile_size_};
}
