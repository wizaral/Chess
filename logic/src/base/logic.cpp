#include "chess.hpp"

namespace Chess {

Logic::Player *ChessGame::get_current_player() {
    return players_[player_index_ %= 2].get();
}

Logic::GameState ChessGame::validate_move(Logic::Move move, Logic::FigureColor color) const {
    if (Logic::Position::validation(move.from()) == false
        || Logic::Position::validation(move.to()) == false) {
        return Logic::GameState::OutOfBounds;
    }

    Logic::Figure *figure = board_.get_figure(move.from());
    if (figure != nullptr) {
        if (figure->color() != color) {
            return Logic::GameState::WrongFigureColor;
        }
        return Logic::GameState::NormalMove;
    }
    return Logic::GameState::EmptySquare;
}

bool ChessGame::is_check(Logic::FigureColor color) const {
    Logic::Position pos = board_.get_position(Logic::FigureType::King, color);
    Logic::FigureColor other = !color;
    return board_.state(other)[pos.row()][pos.col()];
}

bool ChessGame::is_check(Logic::FigureColor color, Logic::Move move) {
    std::optional<Logic::Figure> save;

    if (Logic::Figure *dest = board_.get_figure(move.to()); dest != nullptr)
        save = std::move(*dest);

    board_.move_figure(move);
    update_check_state();

    bool result = is_check(color);

    board_.move_figure({move.to(), move.from()});
    update_check_state();

    if (save.has_value()) {
        board_.add_figure({{}, {}, {}}, move.to());
        *board_.get_figure(move.to()) = std::move(*save);
    }
    return result;
}

void ChessGame::castling(Logic::FigureColor color, Logic::Move move) {
    int row = move.from().row();

    if ((row == Logic::player0_figures_row && color == Logic::FigureColor::Light)
        || (row == Logic::player1_figures_row && color == Logic::FigureColor::Dark)) {
        if (move.cols() == 3) {
            // short
            board_.move_figure({ {row, 4}, {row, 6} }); // king
            board_.move_figure({ {row, 7}, {row, 5} }); // rook
        }
        else /* if (move.cols() == 4) */ {
            // long
            board_.move_figure({ {row, 4}, {row, 2} }); // king
            board_.move_figure({ {row, 0}, {row, 3} }); // rook
        }

        (static_cast<Logic::KingStrategy *>(board_.get_figure({row, 6})->strategy()))->castling_update();
        (static_cast<Logic::KingStrategy *>(board_.get_figure({row, 5})->strategy()))->castling_update();
    } else {
        if (move.cols() == 3) {
            // short
            board_.move_figure({ {row, 3}, {row, 1} }); // king
            board_.move_figure({ {row, 0}, {row, 2} }); // rook
        }
        else /* if (move.cols() == 4) */ {
            // long
            board_.move_figure({ {row, 3}, {row, 5} }); // king
            board_.move_figure({ {row, 7}, {row, 4} }); // rook
        }

        (static_cast<Logic::KingStrategy *>(board_.get_figure({row, 1})->strategy()))->castling_update();
        (static_cast<Logic::RookStrategy *>(board_.get_figure({row, 2})->strategy()))->castling_update();
    }
}

void ChessGame::update_check_state() {
    std::vector<Logic::Position> state_light, state_dark;

    for (int i = 0; i < Logic::board_rows; ++i) {
        for (int j = 0; j < Logic::board_cols; ++j) {
            if (auto f = board_.get_figure({i, j}); f != nullptr) {
                if (f->color() == Logic::FigureColor::Light) {
                    f->strategy()->update_occupation(board_, Logic::Position{i, j}, state_light);
                } else {
                    f->strategy()->update_occupation(board_, Logic::Position{i, j}, state_dark);
                }
            }
        }
    }

    board_.reset_state(Logic::FigureColor::Light);
    board_.update_state(state_light, Logic::FigureColor::Light);

    board_.reset_state(Logic::FigureColor::Dark);
    board_.update_state(state_dark, Logic::FigureColor::Dark);
}

void ChessGame::try_transform_pawns() {
    for (int row : {Logic::player0_figures_row, Logic::player1_figures_row}) {
        for (int i = 0; i < Logic::board_cols; ++i) {
            if (auto f = board_.figures()[row][i].get(); f != nullptr) {
                if (f->type() == Logic::FigureType::Pawn) {
                    transform_pawn({row, i});
                }
            }
        }
    }
}

Logic::GameState ChessGame::is_mate() {
    Logic::FigureColor color = !get_current_player()->color();
    bool state = is_stalemate(color);

    if (is_check(color) && state) {
        return Logic::GameState::CheckMate;
    } else if (state) {
        return Logic::GameState::StaleMate;
    }
    return Logic::GameState::NormalMove;
}

bool ChessGame::is_stalemate(Logic::FigureColor color) {
    for (int i = 0; i < Logic::board_rows; ++i) {
        for (int j = 0; j < Logic::board_cols; ++j) {
            auto figure = board_.get_figure({i, j});

            if (figure != nullptr && figure->color() == color) {
                std::vector<Logic::Position> positions;
                figure->strategy()->update_occupation(board_, {i, j}, positions);

                for (auto pos : positions) {
                    auto dest = board_.get_figure(pos);

                    if (dest == nullptr || dest->color() != color) {
                        if (is_check(color, {{i, j}, pos}) == false) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool ChessGame::is_draw() const {
    std::vector<std::pair<Logic::FigureType, Logic::FigureColor>> figures;

    for (const auto &i : board_.figures()) {
        for (const auto &j : i) {
            if (auto ptr = j.get(); ptr != nullptr) {
                figures.push_back({ptr->type(), ptr->color()});
            }
        }
    }

    if (std::any_of(figures.begin(), figures.end(), [](auto f) {
        return f.first == Logic::FigureType::Queen || f.first == Logic::FigureType::Rook || f.first == Logic::FigureType::Pawn;
    })) {
        return false;
    }

    figures.erase(std::remove_if(figures.begin(), figures.end(), [](auto f) {
        return f.first == Logic::FigureType::King;
    }), figures.end());

    if (size_t size = figures.size(); size == 0 || size == 1) {
        return true;
    } else if (size == 2 && figures[0].second != figures[1].second) /* diff colors */ {
        return true;
    }
    return false;
}

Logic::GameState ChessGame::logic(Logic::Move move) {
    Logic::FigureColor color = get_current_player()->color();
    Logic::GameState gstate = validate_move(move, color);

    if (is_error(gstate)) {
        return gstate;
    }

    bool check = is_check(color);

    Logic::Figure *figure = board_.get_figure(move.from());
    gstate = figure->strategy()->validate_move(*figure, board_, move);

    if (is_error(gstate)) {
        return gstate;
    }
    if (gstate == Logic::GameState::NormalMove) {
        if (is_check(color, move)) {
            return check ? Logic::GameState::KingInCheck : Logic::GameState::KingWillBeInCheck;
        } else {
            board_.move_figure(move);
        }
    } else /* if (gstate == Logic::GameState::AnyCastling) */ {
        castling(color, move);
    }

    update_check_state();
    try_transform_pawns();

    if (auto state = is_mate(); is_endgame(state)) {
        return state;
    }

    return is_draw() ? Logic::GameState::Draw : gstate;
}

} // namespace Chess
