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

bool Game::is_check(FigureColor color, Move move) {
    std::optional<Figure> save;

    if (Figure *dest = board_.get_figure(move.to()); dest != nullptr)
        save = std::move(*dest);

    board_.move_figure(move);
    update_check_state();

    bool result = is_check(color);

    board_.move_figure({move.to(), move.from()});
    update_check_state();

    if (save.has_value()) {
        board_.add_figure({{}, color, std::make_unique<Strategy>()}, move.to());
        *board_.get_figure(move.to()) = std::move(*save);
    }
    return result;
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

    board_.reset_state(FigureColor::Light);
    board_.update_state(state_light, FigureColor::Light);

    board_.reset_state(FigureColor::Dark);
    board_.update_state(state_dark, FigureColor::Dark);
}

void Game::try_transform_pawns() {
    for (int row : {player0_figures_row, player1_figures_row}) {
        for (int i = 0; i < board_cols; ++i) {
            if (auto f = board_.figures()[row][i].get(); f != nullptr) {
                if (f->type() == FigureType::Pawn) {
                    transform_pawn({row, i});
                }
            }
        }
    }
}

void Game::transform_pawn(Position pos) {
    render_->show_pawn_promotion(pos);
    FigureType ft = input_->promote_figure(get_current_player(), pos);
    FigureColor fc = get_current_player()->color();

    switch (ft) {
    case FigureType::Queen:
        board_.add_figure({ft, fc, std::make_unique<Strategy>(QueenStrategy())}, pos);
        break;
    case FigureType::Rook:
        board_.add_figure({ft, fc, std::make_unique<Strategy>(RookStrategy())}, pos);
        break;
    case FigureType::Knight:
        board_.add_figure({ft, fc, std::make_unique<Strategy>(KnightStrategy())}, pos);
        break;
    case FigureType::Bishop:
        board_.add_figure({ft, fc, std::make_unique<Strategy>(BishopStrategy())}, pos);
        break;
    }
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
