#include "board.hpp"
#include "figure_king.hpp"
#include "figure_rook.hpp"

namespace Chess {

King::King(FigureType type, FigureColor color, MoveState state)
: Figure(type, color)
, m_state(state) {}

GameState King::validate_move(const Board &board, Move move) const {
    auto other = board.get_figure(move.to());
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 1 || cols == 1) {
        if (other == nullptr || other->color() != color()) {
            return GameState::NormalMove;
        }
        return GameState::OtherFigureOnPath;
    }
    if (other == nullptr && rows == 0 && cols == 2) {
        other = board.get_figure({move.from().row(), move.from().col() < move.to().col() ? 7 : 0});
        if (other != nullptr) {
            return check_castling(board, move, other);
        }
    }
    return GameState::WrongFigureMove;
}

GameState King::check_castling(const Board &board, Move move, const Figure *other) const {
    auto other_color = !color();

    // check first figure move
    if (m_state == MoveState::NoMove && static_cast<const Rook *>(other)->state() == Rook::MoveState::NoMove) {
        const auto &state = board.get_check_state(other_color);
        int row = move.from().row();

        if (state[row][4] == true) {
            return GameState::CastlingKingInCheck;
        }

        if (int distance = move.from().col() - move.to().col(); distance < 0) {
            for (int col = 5; col < 7; ++col) {
                if (state[row][col] == true) {
                    return GameState::CastlingSquareInCheck;
                }
                if (board.get_figure({row, col}) != nullptr) {
                    return GameState::CastlingFigureOnPath;
                }
            }
            return GameState::KingCastling;
        } else /* if (distance > 0) */ {
            for (int col = 3; col > 1; --col) {
                if (state[row][col] == true) {
                    return GameState::CastlingSquareInCheck;
                }
                if (board.get_figure({row, col}) != nullptr) {
                    return GameState::CastlingFigureOnPath;
                }
            }
            return board.get_figure({row, 1}) == nullptr ? GameState::QueenCastling : GameState::CastlingFigureOnPath;
        }
    }
    return GameState::FiguresAlreadyMoved;
}

void King::update_occupation(const Board &board, Position pos, std::vector<Position> &coords) const {
    static_cast<void>(board);
    int row = pos.row(), col = pos.col();

    std::array<Position, 8> positions{
        Position{row + 1, col + 1},
        Position{row + 1, col - 1},
        Position{row - 1, col + 1},
        Position{row - 1, col - 1},
        Position{row + 1, col + 0},
        Position{row - 1, col - 0},
        Position{row + 0, col + 1},
        Position{row - 0, col - 1},
    };

    for (auto p : positions) {
        if (Position::validation(p)) {
            coords.push_back(p);
        }
    }
}

void King::update_movement(const Board &board, Position pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void King::move_update(Move move) {
    static_cast<void>(move);
    m_state = MoveState::NormalMove;
}

} // namespace Chess
