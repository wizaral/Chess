#include "game.hpp"

Game::Game(sf::RenderWindow &window)
    : window_(window) {
    load();
}

void Game::load() {
    load(rect_, "res" DIR_SEP "rect.png");
    load(board_, "res" DIR_SEP "board.png");
    load(background_, "res" DIR_SEP "bg.jpg");

    load(promotions_[0], "res" DIR_SEP "wPp.png");
    load(promotions_[1], "res" DIR_SEP "bPp.png");

    load(buttons_[0], "res" DIR_SEP "pcb.png");
    load(buttons_[1], "res" DIR_SEP "psb.png");
    load(buttons_[2], "res" DIR_SEP "pcw.png");
    load(buttons_[3], "res" DIR_SEP "psw.png");
    load(buttons_[4], "res" DIR_SEP "play.png");

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

    board_.first.setPosition(0, 0);
    background_.first.setPosition(0, 0);

    buttons_[0].first.setPosition(100, 100);
    buttons_[1].first.setPosition(450, 100);
    buttons_[2].first.setPosition(100, 400);
    buttons_[3].first.setPosition(450, 400);
    buttons_[4].first.setPosition(130, 700);
}

void Game::load(Sprite &sprt, const std::string &path) {
    auto &[sprite, texture] = sprt;
    texture.loadFromFile(path);
    sprite.setTexture(texture);
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
    print_castling();
}

void Game::log() {
    log_ << ' ';
    log_ << static_cast<char>(move_->from().col() + 97) << static_cast<char>(move_->from().row() + 49);
    log_ << static_cast<char>(move_->to().col() + 97) << static_cast<char>(move_->to().row() + 49);
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
