#include "chess.hpp"

namespace Chess {

Logic::Logic(std::array<std::unique_ptr<Player>, 2> arr)
    : players_(std::move(arr)) {
    validate_players();
}

Player *Logic::player() {
    return players_[player_index_ %= 2].get();
}

const Board &Logic::board() const {
    return board_;
}

void Logic::init_game(std::unique_ptr<FigureFactory> factory) {
    factory->init_game(board_, player_index_, this);
}

void Logic::validate_players() const {
    if (players_[0]->color() == players_[1]->color()) {
        throw std::logic_error("Players have same figure colors");
    }
}

void Logic::subscribe(Subscriber *sub) {
    subs_.push_back(sub);
}

void Logic::unsubscribe(Subscriber *sub) {
    subs_.remove(sub);
}

void Logic::notify() {
    for (auto it = subs_.begin(); it != subs_.end();) {
        if ((*it)->update(Subscriber::MessageType::Notify) == false) {
            it = subs_.erase(it);
        } else {
            ++it;
        }
    }
}

GameState Logic::validate_move(const Move &move, FigureColor color) const {
    if (Position::validation(move.from()) == false || Position::validation(move.to()) == false) {
        return GameState::OutOfBounds;
    }

    if (auto figure = board_.get_figure(move.from()); figure != nullptr) {
        if (figure->color() != color) {
            return GameState::WrongFigureColor;
        }
        return GameState::NormalMove;
    }
    return GameState::EmptySquare;
}

bool Logic::is_check(FigureColor color) const {
    Position pos = board_.get_position(FigureType::King, color);
    return board_.get_check_state(!color)[pos.row()][pos.col()];
}

bool Logic::is_check(FigureColor color, const Move &move) {
    std::optional<Figure> save;

    if (Figure *dest = board_.get_figure(move.to()); dest != nullptr) {
        save = std::move(*dest);
    }

    board_.move_figure(move);
    update_check_state();

    bool result = is_check(color);
    board_.move_figure({move.to(), move.from()});

    if (save.has_value()) {
        board_.add_figure({{}, {}, {}}, move.to());
        *board_.get_figure(move.to()) = std::move(*save);
    }

    update_check_state();
    return result;
}

void Logic::castling(FigureColor color, const Move &move) {
    int row = move.from().row();

    if (int distance = move.from().col() - move.to().col(); distance < 0) {
        // short (king) castling
        board_.move_figure({{row, 4}, {row, 6}}); // king
        board_.move_figure({{row, 7}, {row, 5}}); // rook

        static_cast<KingStrategy *>(board_.get_figure({row, 6})->strategy())->move_update(move);
        static_cast<RookStrategy *>(board_.get_figure({row, 5})->strategy())->move_update(move);
    } else /* if (distance > 0) */ {
        // long (queen) castling
        board_.move_figure({{row, 4}, {row, 2}}); // king
        board_.move_figure({{row, 0}, {row, 3}}); // rook

        static_cast<KingStrategy *>(board_.get_figure({row, 2})->strategy())->move_update(move);
        static_cast<RookStrategy *>(board_.get_figure({row, 3})->strategy())->move_update(move);
    }
}

void Logic::en_passant(FigureColor color, const Move &move) {
    board_.move_figure(move);
    board_.remove_figure({move.from().row(), move.to().col()});
}

void Logic::update_check_state() {
    std::vector<Position> state_white, state_black;

    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            if (auto figure = board_.get_figure({i, j}); figure != nullptr) {
                if (figure->color() == FigureColor::White) {
                    figure->strategy()->update_occupation(board_, {i, j}, state_white);
                } else {
                    figure->strategy()->update_occupation(board_, {i, j}, state_black);
                }
            }
        }
    }

    board_.reset_check_state(FigureColor::White);
    board_.update_check_state(FigureColor::White, state_white);

    board_.reset_check_state(FigureColor::Black);
    board_.update_check_state(FigureColor::Black, state_black);
}

void Logic::update_move_state() {
    std::vector<Position> state_white, state_black;

    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            if (auto figure = board_.get_figure({i, j}); figure != nullptr) {
                if (figure->color() == FigureColor::White) {
                    figure->strategy()->update_movement(*figure, board_, {i, j}, state_white);
                } else {
                    figure->strategy()->update_movement(*figure, board_, {i, j}, state_black);
                }
            }
        }
    }

    board_.reset_move_state(FigureColor::White);
    board_.update_move_state(FigureColor::White, state_white);

    board_.reset_move_state(FigureColor::Black);
    board_.update_move_state(FigureColor::Black, state_black);
}

bool Logic::try_promote_pawn(const Figure &figure, const Position &pos) {
    if (figure.type() == FigureType::Pawn) {
        if ((pos.row() == black_figures_row && figure.color() == FigureColor::White)
            || (pos.row() == white_figures_row && figure.color() == FigureColor::Black)) {
            state_ = GameState::PawnPromotion;
            pawn_pos_ = pos;
            return true;
        }
    }
    return false;
}

GameState Logic::is_mate() {
    FigureColor color = !player()->color();
    bool state = is_stalemate(color);

    if (state) {
        return is_check(color) ? GameState::CheckMate : GameState::StaleMate;
    }
    return GameState::NormalMove;
}

bool Logic::is_stalemate(FigureColor color) {
    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            auto figure = board_.get_figure({i, j});

            if (figure != nullptr && figure->color() == color) {
                std::vector<Position> positions;
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

bool Logic::is_draw() const {
    std::vector<std::pair<FigureType, FigureColor>> figures;

    for (const auto &i : board_.figures()) {
        for (const auto &j : i) {
            if (auto ptr = j.get(); ptr != nullptr) {
                figures.push_back({ptr->type(), ptr->color()});
            }
        }
    }

    if (std::any_of(figures.begin(), figures.end(), [](auto figure) {
        return figure.first == FigureType::Queen
            || figure.first == FigureType::Rook
            || figure.first == FigureType::Pawn;
    })) {
        return false;
    }

    figures.erase(std::remove_if(figures.begin(), figures.end(), [](auto figure) {
        return figure.first == FigureType::King;
    }), figures.end());

    if (size_t size = figures.size(); size < 2) {
        return true;
    } else if (size == 2 && figures[0].second != figures[1].second) /* diff colors */ {
        return true;
    }
    return false;
}

GameState Logic::logic(const Move &move) {
    if (state_ == GameState::PawnPromotion) {
        return GameState::PawnPromotion;
    }

    FigureColor color = player()->color();
    state_ = validate_move(move, color);

    if (is_error(state_)) {
        return state_;
    }

    update_check_state();
    bool check = is_check(color);

    Figure *figure = board_.get_figure(move.from());
    state_ = figure->strategy()->validate_move(*figure, board_, move);

    if (is_error(state_)) {
        return state_;
    }
    if (state_ == GameState::NormalMove) {
        if (is_check(color, move)) {
            return check ? GameState::KingInCheck : GameState::KingWillBeInCheck;
        } else {
            figure->strategy()->move_update(move);
            board_.move_figure(move);
        }
    } else if (state_ == GameState::EnPassant) {
        if (is_check(color, move)) {
            return check ? GameState::KingInCheck : GameState::KingWillBeInCheck;
        } else {
            en_passant(color, move);
        }
    } else /* if (state_ == GameState::[Any]Castling) */ {
        castling(color, move);
    }

    if (try_promote_pawn(*figure, move.to())) {
        return state_;
    }

    after_move_logic();
    return state_;
}

void Logic::after_move_logic() {
    notify();

    if (auto state = is_mate(); is_endgame(state)) {
        state_ = state;
    } else if (is_draw()) {
        state_ = GameState::Draw;
    } else {
        ++player_index_;
    }
}

GameState Logic::promote_pawn(FigureType type) {
    if (pawn_pos_ != Position{-1, -1}) {
        FigureColor color = player()->color();
        PawnStrategy *pawn = static_cast<PawnStrategy *>(board_.get_figure(pawn_pos_)->strategy());
        pawn->update(Subscriber::MessageType::Notify);
        pawn->update(Subscriber::MessageType::Notify);

        switch (type) {
        case FigureType::Queen:
            board_.add_figure({type, color, std::make_unique<QueenStrategy>()}, pawn_pos_);
            break;
        case FigureType::Rook:
            board_.add_figure({type, color, std::make_unique<RookStrategy>()}, pawn_pos_);
            break;
        case FigureType::Knight:
            board_.add_figure({type, color, std::make_unique<KnightStrategy>()}, pawn_pos_);
            break;
        case FigureType::Bishop:
            board_.add_figure({type, color, std::make_unique<BishopStrategy>()}, pawn_pos_);
            break;
        default:
            break;
        }

        pawn_pos_ = {-1, -1};
        state_ = GameState::NormalMove;
        after_move_logic();
        return state_;
    }
}

} // namespace Chess
