#include "chess.hpp"

namespace Chess {

Logic::Logic(std::array<std::unique_ptr<Player>, players_amount> arr)
: m_players(std::move(arr)) {
    validate_players();
}

Player *Logic::player() {
    return m_players[m_player_index &= 1].get();
}

const Board &Logic::board() const {
    return m_board;
}

void Logic::init_game(std::unique_ptr<FigureFactory> factory) {
    factory->init_game(m_board, m_player_index, this);
}

void Logic::validate_players() const {
    if (m_players[0]->color() == m_players[1]->color()) {
        throw std::logic_error("Players have same figure colors");
    }
}

void Logic::subscribe(Subscriber *sub) {
    m_subs.push_back(sub);
}

void Logic::unsubscribe(Subscriber *sub) {
    m_subs.remove(sub);
}

void Logic::notify() {
    for (auto it = m_subs.begin(); it != m_subs.end();) {
        if ((*it)->update(Subscriber::MessageType::Notify) == false) {
            it = m_subs.erase(it);
        } else {
            ++it;
        }
    }
}

GameState Logic::validate_move(const Move &move, FigureColor color) const {
    if (Position::validation(move.from()) == false || Position::validation(move.to()) == false) {
        return GameState::OutOfBounds;
    }

    if (auto figure = m_board.get_figure(move.from()); figure != nullptr) {
        if (figure->color() != color) {
            return GameState::WrongFigureColor;
        }
        return GameState::NormalMove;
    }
    return GameState::EmptySquare;
}

bool Logic::is_check(FigureColor color) const {
    auto pos = m_board.get_position(FigureType::King, color);
    return m_board.get_check_state(!color)[pos.row()][pos.col()];
}

bool Logic::is_check(FigureColor color, const Move &move) {
    auto figure = m_board.extract_figure(move.to());
    m_board.move_figure(move);

    update_check_state();
    bool result = is_check(color);

    m_board.move_figure({move.to(), move.from()});
    m_board.add_figure(std::move(figure), move.to());

    update_check_state();
    return result;
}

void Logic::castling(FigureColor color, const Move &move) {
    int row = move.from().row();

    if (int distance = move.from().col() - move.to().col(); distance < 0) {
        // short (king) castling
        m_board.move_figure({{row, 4}, {row, 6}}); // king
        m_board.move_figure({{row, 7}, {row, 5}}); // rook

        static_cast<King *>(m_board.get_figure({row, 6}))->move_update(move);
        static_cast<Rook *>(m_board.get_figure({row, 5}))->move_update(move);
    } else /* if (distance > 0) */ {
        // long (queen) castling
        m_board.move_figure({{row, 4}, {row, 2}}); // king
        m_board.move_figure({{row, 0}, {row, 3}}); // rook

        static_cast<King *>(m_board.get_figure({row, 2}))->move_update(move);
        static_cast<Rook *>(m_board.get_figure({row, 3}))->move_update(move);
    }
}

void Logic::en_passant(FigureColor color, const Move &move) {
    m_board.move_figure(move);
    m_board.remove_figure({move.from().row(), move.to().col()});
}

void Logic::update_check_state() {
    std::vector<Position> state_white, state_black;

    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            if (auto figure = m_board.get_figure({i, j}); figure != nullptr) {
                if (figure->color() == FigureColor::White) {
                    figure->update_occupation(m_board, {i, j}, state_white);
                } else {
                    figure->update_occupation(m_board, {i, j}, state_black);
                }
            }
        }
    }

    m_board.reset_check_state(FigureColor::White);
    m_board.update_check_state(FigureColor::White, state_white);

    m_board.reset_check_state(FigureColor::Black);
    m_board.update_check_state(FigureColor::Black, state_black);
}

void Logic::update_move_state() {
    std::vector<Position> state_white, state_black;

    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            if (auto figure = m_board.get_figure({i, j}); figure != nullptr) {
                if (figure->color() == FigureColor::White) {
                    figure->update_movement(m_board, {i, j}, state_white);
                } else {
                    figure->update_movement(m_board, {i, j}, state_black);
                }
            }
        }
    }

    m_board.reset_move_state(FigureColor::White);
    m_board.update_move_state(FigureColor::White, state_white);

    m_board.reset_move_state(FigureColor::Black);
    m_board.update_move_state(FigureColor::Black, state_black);
}

bool Logic::try_promote_pawn(const Figure *figure, Position pos) {
    if (figure->type() == FigureType::Pawn) {
        if ((pos.row() == figures_row[static_cast<int>(FigureColor::Black)] && figure->color() == FigureColor::White)
            || (pos.row() == figures_row[static_cast<int>(FigureColor::White)] && figure->color() == FigureColor::Black)) {
            m_state = GameState::PawnPromotion;
            m_pawn_pos = pos;
            return true;
        }
    }
    return false;
}

GameState Logic::is_mate() {
    auto color = !player()->color();

    if (is_stalemate(color)) {
        return is_check(color) ? GameState::CheckMate : GameState::StaleMate;
    }
    return GameState::NormalMove;
}

bool Logic::is_stalemate(FigureColor color) {
    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            auto figure = m_board.get_figure({i, j});

            if (figure != nullptr && figure->color() == color) {
                std::vector<Position> positions;
                figure->update_movement(m_board, {i, j}, positions);

                for (auto pos : positions) {
                    auto dest = m_board.get_figure(pos);

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

    for (const auto &i : m_board.figures()) {
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
    if (m_state == GameState::PawnPromotion) {
        return GameState::PawnPromotion;
    }

    auto color = player()->color();
    m_state = validate_move(move, color);

    if (is_error(m_state)) {
        return m_state;
    }

    update_check_state();
    bool check = is_check(color);

    auto figure = m_board.get_figure(move.from());
    m_state = figure->validate_move(m_board, move);

    if (is_error(m_state)) {
        return m_state;
    }
    if (m_state == GameState::NormalMove) {
        if (is_check(color, move)) {
            return check ? GameState::KingInCheck : GameState::KingWillBeInCheck;
        } else {
            figure->move_update(move);
            m_board.move_figure(move);
        }
    } else if (m_state == GameState::EnPassant) {
        if (is_check(color, move)) {
            return check ? GameState::KingInCheck : GameState::KingWillBeInCheck;
        } else {
            en_passant(color, move);
        }
    } else /* if (m_state == GameState::[Any]Castling) */ {
        castling(color, move);
    }

    if (try_promote_pawn(figure, move.to())) {
        return m_state;
    }

    after_move_logic();
    return m_state;
}

void Logic::after_move_logic() {
    notify();

    if (auto state = is_mate(); is_endgame(state)) {
        m_state = state;
    } else if (is_draw()) {
        m_state = GameState::Draw;
    } else {
        ++m_player_index;
    }
}

GameState Logic::promote_pawn(FigureType type) {
    if (m_pawn_pos != Position{-1, -1}) {
        FigureColor color = player()->color();
        auto pawn = static_cast<Pawn *>(m_board.get_figure(m_pawn_pos));
        pawn->update(Subscriber::MessageType::Notify);
        pawn->update(Subscriber::MessageType::Notify);

        switch (type) {
        case FigureType::Queen:
            m_board.add_figure(std::make_unique<Queen>(type, color), m_pawn_pos);
            break;
        case FigureType::Rook:
            m_board.add_figure(std::make_unique<Rook>(type, color), m_pawn_pos);
            break;
        case FigureType::Knight:
            m_board.add_figure(std::make_unique<Knight>(type, color), m_pawn_pos);
            break;
        case FigureType::Bishop:
            m_board.add_figure(std::make_unique<Bishop>(type, color), m_pawn_pos);
            break;
        default:
            break;
        }

        m_pawn_pos = {-1, -1};
        m_state = GameState::NormalMove;
        after_move_logic();
    }
    return m_state;
}

std::vector<Position> Logic::possible_moves(Position pos) {
    std::vector<Position> moves;

    if (auto figure = m_board.get_figure(pos); figure != nullptr) {
        figure->update_movement(m_board, pos, moves);

        moves.erase(std::remove_if(moves.begin(), moves.end(), [this, figure, pos](const auto &move) {
            return is_error(figure->validate_move(m_board, {pos, move}));
        }), moves.end());
    }
    return moves;
}

std::vector<Position> Logic::possible_moves(Position pos, FigureColor color) {
    std::vector<Position> moves;

    if (auto figure = m_board.get_figure(pos); figure != nullptr) {
        figure->update_movement(m_board, pos, moves);

        moves.erase(std::remove_if(moves.begin(), moves.end(), [this, figure, pos, color](const auto &move) {
            return is_check(color, {pos, move}) || is_error(figure->validate_move(m_board, {pos, move})) || figure->color() != color;
        }), moves.end());
    }
    return moves;
}

} // namespace Chess
