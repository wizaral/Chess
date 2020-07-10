#include "game.hpp"

namespace Chess::Logic {

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
    // if (move.from().row() == player0_figures_row) {
    //     if (color == FigureColor::Light) {
    //         board_.move_figure({{player0_figures_row, 4}, {player0_figures_row, 6}});   // king
    //         board_.move_figure({{player0_figures_row, 7}, {player0_figures_row, 5}});   // rook

    //         KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({player0_figures_row, 6})->strategy());
    //         kst->castling_update();
    //         RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({player0_figures_row, 5})->strategy());
    //         rst->castling_update();
    //     } else {
    //         board_.move_figure({{player0_figures_row, 3}, {player0_figures_row, 1}});   // king
    //         board_.move_figure({{player0_figures_row, 0}, {player0_figures_row, 2}});   // rook

    //         // (static_cast<KingStrategy *>(board_.get_figure({player0_figures_row, 6})->strategy()))->castling_update();
    //         KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({player0_figures_row, 1})->strategy());
    //         kst->castling_update();

    //         // (static_cast<RookStrategy *>(board_.get_figure({player0_figures_row, 5})->strategy()))->castling_update();
    //         RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({player0_figures_row, 2})->strategy());
    //         rst->castling_update();
    //     }
    // } else /* if (move.from().row() == player1_figures_row) */ {
    //     if (color == FigureColor::Dark) {
    //         board_.move_figure({{player1_figures_row, 4}, {player1_figures_row, 6}});   // king
    //         board_.move_figure({{player1_figures_row, 7}, {player1_figures_row, 5}});   // rook

    //         KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({player1_figures_row, 6})->strategy());
    //         kst->castling_update();
    //         RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({player1_figures_row, 5})->strategy());
    //         rst->castling_update();
    //     } else {
    //         board_.move_figure({{player1_figures_row, 3}, {player1_figures_row, 1}});   // king
    //         board_.move_figure({{player1_figures_row, 0}, {player1_figures_row, 2}});   // rook

    //         KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({player1_figures_row, 1})->strategy());
    //         kst->castling_update();
    //         RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({player1_figures_row, 2})->strategy());
    //         rst->castling_update();
    //     }
    // }

    int row = move.from().row();
    if ((row == player0_figures_row && color == FigureColor::Light)
        || (move.from().row() == player1_figures_row && color == FigureColor::Dark)) {
        board_.move_figure({{row, 4}, {row, 6}});   // king
        board_.move_figure({{row, 7}, {row, 5}});   // rook

        // (static_cast<KingStrategy *>(board_.get_figure({row, 6})->strategy()))->castling_update();
        KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({row, 6})->strategy());
        kst->castling_update();
        // (static_cast<KingStrategy *>(board_.get_figure({row, 5})->strategy()))->castling_update();
        RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({row, 5})->strategy());
        rst->castling_update();
    } else {
        board_.move_figure({{row, 3}, {row, 1}});   // king
        board_.move_figure({{row, 0}, {row, 2}});   // rook

        // (static_cast<KingStrategy *>(board_.get_figure({row, 1})->strategy()))->castling_update();
        KingStrategy *kst = static_cast<KingStrategy *>(board_.get_figure({row, 1})->strategy());
        kst->castling_update();
        // (static_cast<RookStrategy *>(board_.get_figure({row, 2})->strategy()))->castling_update();
        RookStrategy *rst = static_cast<RookStrategy *>(board_.get_figure({row, 2})->strategy());
        rst->castling_update();
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

bool Game::is_mate() {
    FigureColor color = get_current_player()->color() == FigureColor::Light ? FigureColor::Dark : FigureColor::Light;
    return is_check(color) && is_stalemate();
}

bool Game::is_stalemate() {
    FigureColor color = get_current_player()->color() == FigureColor::Light ? FigureColor::Dark : FigureColor::Light;

    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            auto figure = board_.get_figure({i, j});

            if (figure != nullptr && figure->color() == color) {
                std::vector<Position> positions;
                figure->strategy()->update_occupation(board_, {i, j}, positions);

                for (auto pos : positions) {
                    auto dest = board_.get_figure({i, j});

                    if (dest != nullptr || dest->color() != color) {
                        if (!is_check(color, {{i, j}, pos})) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool Game::is_draw() const {
    std::vector<std::pair<FigureType, FigureColor>> figures;

    for (const auto &i : board_.figures()) {
        for (const auto &j : i) {
            if (auto ptr = j.get(); ptr != nullptr) {
                figures.push_back({ptr->type(), ptr->color()});
            }
        }
    }

    if (std::any_of(figures.begin(), figures.end(), [](auto f) {
        return f.first == FigureType::Queen || f.first == FigureType::Rook || f.first == FigureType::Pawn;
    })) {
        return false;
    }

    std::remove_if(figures.begin(), figures.end(), [](auto f) {
        return f.first == FigureType::King;
    });

    if (size_t size = figures.size(); size == 0 || size == 1) {
        return true;
    } else if (size == 2 && figures[0].second != figures[1].second) /* diff colors */ {
        return true;
    }
    return false;
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
    } else /* if (gstate == GameState::AnyCastling) */ {
        castling(color, move);
    }

    update_check_state();
    try_transform_pawns();

    if (is_mate())
        return GameState::CheckMate;
    if (is_draw())
        return GameState::Draw;
    return gstate;
}

}
