#include "game.hpp"

Game::Game(sf::RenderWindow &window)
    : window_(window) {
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

void Game::reprint() {
    window_.clear();
    print_board();
    print_next_step();
}

void Game::loop() {
    while (window_.isOpen() && !Chess::is_endgame(state_)) {
        reprint();

        if (state_ == Chess::GameState::PawnPromotion) {
            pawn_promotion();
        } else {
            logic_->player()->get_next_move();
        }

        if (move_.has_value()) {
            state_ = logic_->logic(move_.value());

            if (Chess::is_move(state_)) {
                log();
            }

            if (state_ == Chess::GameState::KingCastling || state_ == Chess::GameState::QueenCastling) {
                castling();
            }

            if (state_ == Chess::GameState::PawnPromotion) {
                promoting_ = move_->to();
            }
            reprint();
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
        char ftype = '\0';

        switch (auto type = logic_->board().get_figure(promoting_)->type(); type) {
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

        log_ << ftype;
        promoting_ = Chess::Position{-1, -1};
    }
}

void Game::castling() {
    int row = move_->from().row();

    if (int distance = move_->from().col() - move_->to().col(); distance < 0) {
        move_ = Chess::Move{{row, 7}, {row, 5}};
    } else /* if (distance > 0) */ {
        move_ = Chess::Move{{row, 0}, {row, 3}};
    }

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

void Game::log() {
    log_ << ' ';
    log_ << static_cast<char>(move_->from().col() + 97) << static_cast<char>(move_->from().row() + 49);
    log_ << static_cast<char>(move_->to().col() + 97) << static_cast<char>(move_->to().row() + 49);
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

bool Game::is_exit(const sf::Event &e) {
    return e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape);
}

Chess::Position Game::transform(const sf::Vector2i &pos) {
    int off = static_cast<int>(tile_size_);
    return {7 - pos.y / off, pos.x / off};
}

sf::Vector2f Game::transform(const Chess::Position &pos) {
    return {pos.col() * tile_size_, (7 - pos.row()) * tile_size_};
}
