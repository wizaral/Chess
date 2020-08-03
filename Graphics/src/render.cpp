#include "g_render.hpp"
#include <iostream>

GraphicsRender::GraphicsRender(sf::RenderWindow &window) : window_(window) {
    load(board_, "res" DIR_SEP "board.png");
    board_.first.move(sf::Vector2f{0, 100});

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
    window_.clear();
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
    window_.display();
}

void GraphicsRender::show_next_step(Chess::Logic::Player *player) {
    //std::cout << player->name() << " move: ";
}

void GraphicsRender::show_error(Chess::Logic::GameState state) {
    //std::cout << "Error: " << states.at(state) << std::endl;
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
    //switch (state) {
    //case Chess::Logic::GameState::CheckMate:
    //    std::cout << "Player " << winer->name() << " has won!" << std::endl;
    //    break;
    //case Chess::Logic::GameState::StaleMate:
    //    std::cout << "Stalemate. Game over!" << std::endl;
    //    break;
    //default:
    //    std::cout << "Draw. Game over!" << std::endl;
    //    break;
    //}
}

Chess::Logic::Position GraphicsRender::transform(const sf::Vector2i& pos) {
    int off = static_cast<int>(offset);
    Chess::Logic::Position p{ 8 - pos.y / off, pos.x / off };
    std::cout << "Pos " << p.row() << ":" << p.col() << std::endl;
    return p;
}

sf::Vector2f GraphicsRender::transform(const Chess::Logic::Position& pos) {
    return { pos.col() * offset, (8 - pos.row()) * offset };
}
