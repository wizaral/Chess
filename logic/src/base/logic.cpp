#include "game.hpp"

namespace Chess::Logic {

void Game::init_game() {
    FigureColor p0c = players_[0]->color(), p1c = players_[1]->color();

    if (p0c == FigureColor::Light) {
        board_.add_figure({FigureType::Queen, FigureColor::Light, std::make_unique<Strategy>(QueenStrategy())}, {player0_figures_row, 3});
        board_.add_figure({FigureType::King, FigureColor::Light, std::make_unique<Strategy>(KingStrategy())}, {player0_figures_row, 4});
        board_.add_figure({FigureType::Queen, FigureColor::Dark, std::make_unique<Strategy>(QueenStrategy())}, {player1_figures_row, 3});
        board_.add_figure({FigureType::King, FigureColor::Dark, std::make_unique<Strategy>(KingStrategy())}, {player1_figures_row, 4});
    } else {
        board_.add_figure({FigureType::Queen, FigureColor::Dark, std::make_unique<Strategy>(QueenStrategy())}, {player0_figures_row, 4});
        board_.add_figure({FigureType::King, FigureColor::Dark, std::make_unique<Strategy>(KingStrategy())}, {player0_figures_row, 3});
        board_.add_figure({FigureType::Queen, FigureColor::Light, std::make_unique<Strategy>(QueenStrategy())}, {player1_figures_row, 4});
        board_.add_figure({FigureType::King, FigureColor::Light, std::make_unique<Strategy>(KingStrategy())}, {player1_figures_row, 3});
    }

    spawn_figures(player0_figures_row, p0c);
    spawn_pawns(player0_pawns_row, p0c, player0_step_direction);

    spawn_figures(player1_figures_row, p1c);
    spawn_pawns(player1_pawns_row, p1c, player1_step_direction);
}

void Game::validate() const {
    if (players_[0]->color() == players_[1]->color())
        throw std::logic_error("Players have same figure colors");
}

Player *Game::get_current_player() {
    return players_[player_index_ %= 2].get();
}

GameState Game::validate_move(Move move, FigureColor color) const {
    try {
        Position::validation(move.from());
        Position::validation(move.to());
    } catch (std::out_of_range) {
        return GameState::OutOfBounds;
    }

    Figure *figure = board_.get_figure(move.from());
    if (figure != nullptr) {
        if (figure->color() != color) {
            return GameState::WrongFigureColor;
        }
        return GameState::NormalMove;
    }
    return GameState::EmptySquare;
}

bool Game::is_check(FigureColor color) const {
    Position pos = board_.get_position(FigureType::King, color);
    FigureColor other = color == FigureColor::Light ? FigureColor::Dark : FigureColor::Light;
    return board_.state(other)[pos.row()][pos.col()];
}

bool Game::is_check(FigureColor color, Move move) const {
    // FigureColor other = color == FigureColor::Light ? FigureColor::Dark : FigureColor::Light;
    // // std::array<std::array<bool, 8>, 8> state = board_.state(other);
    // // auto state = board_.state(other);

    // std::array<std::array<bool, 8>, 8> state;
    // std::copy(board_.state(other).begin(), board_.state(other).end(), state.begin());
}

void Game::castling(FigureColor color, Move move) {
    if (move.from().row() == player0_figures_row) {
        if (color == FigureColor::Light) {
            board_.move_figure({{player0_figures_row, 4}, {player0_figures_row, 6}});
            board_.move_figure({{player0_figures_row, 7}, {player0_figures_row, 5}});

            KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({player0_figures_row, 6})->strategy());
            kst->castling_update();
            RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({player0_figures_row, 5})->strategy());
            rst->castling_update();
        } else {
            board_.move_figure({{player0_figures_row, 4}, {player0_figures_row, 6}});
            board_.move_figure({{player0_figures_row, 7}, {player0_figures_row, 5}});

            // (static_cast<KingStrategy *>(board_.get_figure({player0_figures_row, 6})->strategy()))->castling_update();
            KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({player0_figures_row, 6})->strategy());
            kst->castling_update();

            // (static_cast<RookStrategy *>(board_.get_figure({player0_figures_row, 5})->strategy()))->castling_update();
            RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({player0_figures_row, 5})->strategy());
            rst->castling_update();
        }
    } else /* if (move.from().row() == player1_figures_row) */ {

    }
}

void Game::update_check_state() {
    std::vector<Position> state_light, state_dark;

    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            if (auto f = board_.figures()[i][j].get(); f != nullptr) {
                if (f->color() == FigureColor::Light) {
                    f->strategy()->update_occupation(board_, {i, j}, state_light);
                } else {
                    f->strategy()->update_occupation(board_, {i, j}, state_dark);
                }
            }
        }
    }

    board_.update_state(state_light, FigureColor::Light);
    board_.update_state(state_dark, FigureColor::Dark);
}

void Game::try_transform_pawns() {

}

void Game::transform_pawn(Logic::Position pos) {

}

bool Game::is_mate() const {

}

bool Game::is_draw() const {

}

GameState Game::logic(Move move) {
    FigureColor color = get_current_player()->color();
    GameState gstate = validate_move(move, color);
    bool check = is_check(color);

    Figure *figure = board_.get_figure(move.from());
    gstate = figure->strategy()->validate_move(*figure, board_, move);

    if (is_error(gstate)) {
        return gstate;
    }
    if (gstate == GameState::NormalMove) {
        if (is_check(color, move)) {
            return check ? GameState::KingInCheck : GameState::KingWillBeInCheck;
        } else {
            board_.move_figure(move);
        }
    } else /* if (gstate == GameState::*Castling) */ {
        castling(color, move);
    }

    update_check_state();
    try_transform_pawns();

    // if (is_mate())
    //     return GameState::CheckMate;
    // if (is_draw())
    //     return GameState::Draw;
    return gstate;
}

}
