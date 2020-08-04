#include "chess.hpp"

namespace Chess {

Logic::GameState ChessGame::validate_move(const Logic::Move &move, Logic::FigureColor color) const {
    if (Logic::Position::validation(move.from()) == false || Logic::Position::validation(move.to()) == false) {
        return Logic::GameState::OutOfBounds;
    }

    if (auto figure = board_.get_figure(move.from()); figure != nullptr) {
        if (figure->color() != color) {
            return Logic::GameState::WrongFigureColor;
        }
        return Logic::GameState::NormalMove;
    }
    return Logic::GameState::EmptySquare;
}

bool ChessGame::is_check(Logic::FigureColor color) const {
    Logic::Position pos = board_.get_position(Logic::FigureType::King, color);
    return board_.get_check_state(!color)[pos.row()][pos.col()];
}

bool ChessGame::is_check(Logic::FigureColor color, const Logic::Move &move) {
    std::optional<Logic::Figure> save;

    if (Logic::Figure *dest = board_.get_figure(move.to()); dest != nullptr) {
        save = std::move(*dest);
    }

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

void ChessGame::castling(Logic::FigureColor color, const Logic::Move &move) {
    int row = move.from().row();

    if (move.cols() == 3) {
        // short (king) castling
        board_.move_figure({{row, 4}, {row, 6}}); // king
        board_.move_figure({{row, 7}, {row, 5}}); // rook

        static_cast<Logic::KingStrategy *>(board_.get_figure({row, 6})->strategy())->move_update(move);
        static_cast<Logic::RookStrategy *>(board_.get_figure({row, 5})->strategy())->move_update(move);
    } else /* if (move.cols() == 4) */ {
        // long (queen) castling
        board_.move_figure({{row, 4}, {row, 2}}); // king
        board_.move_figure({{row, 0}, {row, 3}}); // rook

        static_cast<Logic::KingStrategy *>(board_.get_figure({row, 2})->strategy())->move_update(move);
        static_cast<Logic::RookStrategy *>(board_.get_figure({row, 3})->strategy())->move_update(move);
    }
}

void ChessGame::en_passant(Logic::FigureColor color, const Logic::Move &move) {
    board_.move_figure(move);
    board_.remove_figure({move.from().row(), move.to().col()});
}

void ChessGame::update_check_state() {
    std::vector<Logic::Position> state_white, state_black;

    for (int i = 0; i < Logic::board_rows; ++i) {
        for (int j = 0; j < Logic::board_cols; ++j) {
            if (auto figure = board_.get_figure({i, j}); figure != nullptr) {
                if (figure->color() == Logic::FigureColor::White) {
                    figure->strategy()->update_occupation(board_, Logic::Position{i, j}, state_white);
                } else {
                    figure->strategy()->update_occupation(board_, Logic::Position{i, j}, state_black);
                }
            }
        }
    }

    board_.reset_check_state(Logic::FigureColor::White);
    board_.update_check_state(state_white, Logic::FigureColor::White);

    board_.reset_check_state(Logic::FigureColor::Black);
    board_.update_check_state(state_black, Logic::FigureColor::Black);
}

void ChessGame::update_move_state() {
    std::vector<Logic::Position> state_white, state_black;

    for (int i = 0; i < Logic::board_rows; ++i) {
        for (int j = 0; j < Logic::board_cols; ++j) {
            if (auto figure = board_.get_figure({i, j}); figure != nullptr) {
                if (figure->color() == Logic::FigureColor::White) {
                    figure->strategy()->update_movement(*figure, board_, Logic::Position{i, j}, state_white);
                } else {
                    figure->strategy()->update_movement(*figure, board_, Logic::Position{i, j}, state_black);
                }
            }
        }
    }

    board_.reset_move_state(Logic::FigureColor::White);
    board_.update_move_state(state_white, Logic::FigureColor::White);

    board_.reset_move_state(Logic::FigureColor::Black);
    board_.update_move_state(state_black, Logic::FigureColor::Black);
}

bool ChessGame::try_promote_pawn(const Logic::Figure &figure, const Logic::Position &pos) {
    if (figure.type() == Logic::FigureType::Pawn){
        if ((pos.row() == Logic::black_figures_row && figure.color() == Logic::FigureColor::White)
            || (pos.row() == Logic::white_figures_row && figure.color() == Logic::FigureColor::Black)) {
            state_ = Logic::GameState::PawnPromotion;
            pawn_pos_ = pos;
            return true;
        }
    }
    return false;
}

Logic::GameState ChessGame::is_mate() {
    Logic::FigureColor color = !player()->color();
    bool state = is_stalemate(color);

    if (state) {
        return is_check(color) ? Logic::GameState::CheckMate : Logic::GameState::StaleMate;
    }
    return Logic::GameState::NormalMove;
}

bool ChessGame::is_stalemate(Logic::FigureColor color) {
    for (int i = 0; i < Logic::board_rows; ++i) {
        for (int j = 0; j < Logic::board_cols; ++j) {
            auto figure = board_.get_figure({i, j});

            if (figure != nullptr && figure->color() == color) {
                std::vector<Logic::Position> positions;
                figure->strategy()->update_movement(*figure, board_, {i, j}, positions);

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

    if (std::any_of(figures.begin(), figures.end(), [](auto figure) {
        return figure.first == Logic::FigureType::Queen
            || figure.first == Logic::FigureType::Rook
            || figure.first == Logic::FigureType::Pawn;
    })) {
        return false;
    }

    figures.erase(std::remove_if(figures.begin(), figures.end(), [](auto figure) {
        return figure.first == Logic::FigureType::King;
    }), figures.end());

    if (size_t size = figures.size(); size < 2) {
        return true;
    } else if (size == 2 && figures[0].second != figures[1].second) /* diff colors */ {
        return true;
    }
    return false;
}

Logic::GameState ChessGame::logic(const Logic::Move &move) {
    if (state_ == Logic::GameState::PawnPromotion) {
        return Logic::GameState::PawnPromotion;
    }

    Logic::FigureColor color = player()->color();
    state_ = validate_move(move, color);

    if (is_error(state_)) {
        return state_;
    }

    update_check_state();
    bool check = is_check(color);

    Logic::Figure *figure = board_.get_figure(move.from());
    state_ = figure->strategy()->validate_move(*figure, board_, move);

    if (is_error(state_)) {
        return state_;
    }
    if (state_ == Logic::GameState::NormalMove) {
        if (is_check(color, move)) {
            return check ? Logic::GameState::KingInCheck : Logic::GameState::KingWillBeInCheck;
        } else {
            figure->strategy()->move_update(move);
            board_.move_figure(move);
        }
    } else if (state_ == Logic::GameState::EnPassant) {
        if (is_check(color, move)) {
            return check ? Logic::GameState::KingInCheck : Logic::GameState::KingWillBeInCheck;
        } else {
            en_passant(color, move);
        }
    } else /* if (state_ == Logic::GameState::[Any]Castling) */ {
        castling(color, move);
    }

    if (try_promote_pawn(*figure, move.to())) {
        return state_;
    }

    after_move_logic();
    return state_;
}

void ChessGame::after_move_logic() {
    notify();

    if (auto state = is_mate(); is_endgame(state)) {
        state_ = state;
    } else if (is_draw()) {
        state_ = Logic::GameState::Draw;
    } else {
        ++player_index_;
    }
}

Logic::GameState ChessGame::promote_pawn(Logic::FigureType type) {
    Logic::FigureColor color = player()->color();
    Logic::PawnStrategy *pawn = static_cast<Logic::PawnStrategy *>(board_.get_figure(pawn_pos_)->strategy());
    pawn->update(Subscriber::MessageType::Notify);
    pawn->update(Subscriber::MessageType::Notify);

    switch (type) {
    case Logic::FigureType::Queen:
        board_.add_figure({type, color, std::make_unique<Logic::QueenStrategy>()}, pawn_pos_);
        break;
    case Logic::FigureType::Rook:
        board_.add_figure({type, color, std::make_unique<Logic::RookStrategy>()}, pawn_pos_);
        break;
    case Logic::FigureType::Knight:
        board_.add_figure({type, color, std::make_unique<Logic::KnightStrategy>()}, pawn_pos_);
        break;
    case Logic::FigureType::Bishop:
        board_.add_figure({type, color, std::make_unique<Logic::BishopStrategy>()}, pawn_pos_);
        break;
    default:
        break;
    }

    pawn_pos_ = {-1, -1};
    after_move_logic();
    return state_;
}

} // namespace Chess
