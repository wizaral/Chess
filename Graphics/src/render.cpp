#include "g_render.hpp"
#include <sstream>

GraphicsRender::GraphicsRender(sf::RenderWindow &window) : window_(window) {
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

void GraphicsRender::load(Sprite &sprt, const std::string &path) {
    auto &[sprite, texture] = sprt;
    texture.loadFromFile(path);
    sprite.setTexture(texture);
}

void GraphicsRender::show_menu() {
    //std::cout << "THIS IS MENU" << std::endl;
}

void GraphicsRender::show_board(const Chess::Logic::Board &board) {
    window_.draw(board_.first);

    for (int row = Chess::Logic::board_rows - 1; row > -1; --row) {
        for (int col = 0; col < Chess::Logic::board_cols; ++col) {
            if (auto f = board.get_figure({row, col}); f != nullptr) {
                auto &figure = figures_[static_cast<int>(f->color())][static_cast<int>(f->type())].first;
                figure.setPosition(transform(Chess::Logic::Position{ row, col }));
                window_.draw(figure);
            }
        }
    }
}

void GraphicsRender::show_next_step(Chess::Logic::Player *player) {
    std::ostringstream oss;
    oss << player->name() << " move: ";
    sf::Text text(oss.str(), font_);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 810.f);
    window_.draw(text);
    window_.display();
}

void GraphicsRender::show_error(Chess::Logic::GameState state) {
    window_.clear();
    if (Chess::Logic::is_error(state)) {
        std::ostringstream oss;
        oss << "Error: " << states.at(state);
        sf::Text text(oss.str(), font_);
        text.setFillColor(sf::Color(255, 255, 255));
        text.setPosition(10.f, 860.f);
        window_.draw(text);
    }
}

void GraphicsRender::show_pawn_promotion(const Chess::Logic::Board& board, const Chess::Logic::Position &pos) {
    window_.clear();
    window_.draw(board_.first);

    for (int row = Chess::Logic::board_rows - 1; row > -1; --row) {
        for (int col = 0; col < Chess::Logic::board_cols; ++col) {
            if (auto f = board.get_figure({ row, col }); f != nullptr) {
                if (pos.row() == row && pos.col() == col) {
                    auto& promotion = promotions_[static_cast<int>(f->color())].first;
                    promotion.setPosition(transform(Chess::Logic::Position{ row, col }));
                    window_.draw(promotion);
                } else {
                    auto& figure = figures_[static_cast<int>(f->color())][static_cast<int>(f->type())].first;
                    figure.setPosition(transform(Chess::Logic::Position{ row, col }));
                    window_.draw(figure);
                }
            }
        }
    }
    window_.display();
}

void GraphicsRender::show_endgame(Chess::Logic::GameState state, Chess::Logic::Player *winer) {
    std::ostringstream oss;
    oss << "Game over: ";

    switch (state) {
    case Chess::Logic::GameState::CheckMate:
        oss << "Player " << winer->name() << " won!";
        break;
    case Chess::Logic::GameState::StaleMate:
        oss << "Stalemate!";
        break;
    default:
        oss << "Draw!";
        break;
    }

    sf::Text text(oss.str(), font_);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition(10.f, 810.f);
    window_.draw(text);
    window_.display();
}

Chess::Logic::Position GraphicsRender::transform(const sf::Vector2i& pos) {
    int off = static_cast<int>(tile_size);
    return { 7 - pos.y / off, pos.x / off };
}

sf::Vector2f GraphicsRender::transform(const Chess::Logic::Position& pos) {
    return { pos.col() * tile_size, (7 - pos.row()) * tile_size };
}
