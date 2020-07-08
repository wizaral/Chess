#include "strategy.hpp"

namespace Chess::Logic {

MoveType KingStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    Figure *other = board.get_figure(move.to());
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 1 || cols == 1) {
        if (other == nullptr || other->color() != figure.color()) {
            state_ = MoveState::NormalMove;
            return MoveType::True;
        }
    } else if (other != nullptr && other->type() == FigureType::Rook && other->color() == figure.color()) {
        return check_castling(figure, board, move, other);
    }
    return MoveType::False;
}

MoveType KingStrategy::check_castling(const Figure &figure, const Board &board, const Move &move, Figure *other) {
    FigureColor other_color = figure.color() == FigureColor::Light ? FigureColor::Dark : FigureColor::Light;
    RookStrategy *rst = static_cast<RookStrategy *>(other->strategy());

    // check first figure move
    if (state_ == MoveState::NoMove && rst->state() == RookStrategy::MoveState::NoMove) {
        const std::array<std::array<bool, board_rows>, board_cols> &state = board.state(other_color);
        int distance = move.cols();
        int row = move.from().row();

        if ((row == 0 && figure.color() == FigureColor::Light) || (row == 7 && figure.color() == FigureColor::Dark)) {
            if (distance == 3) {
                if (state[row][4] == true) {
                    return MoveType::False;
                }

                for (int col = 5; col < 7; ++col) {
                    if (state[row][col] == true || board.get_figure({row, col}) != nullptr) {
                        return MoveType::False;
                    }
                }
                return MoveType::KingCastling;
            } else /*if (distance == 4)*/ {
                if (state[row][4] == true) {
                    return MoveType::False;
                }

                for (int col = 3; col > 1; --col) {
                    if (state[row][col] == true || board.get_figure({row, col}) != nullptr) {
                        return MoveType::False;
                    }
                }
                return board.get_figure({row, 1}) == nullptr ? MoveType::QueenCastling : MoveType::False;
            }
        } else /* if ((row == 0 && figure.color() == FigureColor::Dark) || (row == 7 && figure.color() == FigureColor::Light))*/ {
            if (distance == 3) {
                if (state[row][3] == true) {
                    return MoveType::False;
                }

                for (int col = 2; col > 0; --col) {
                    if (state[row][col] == true || board.get_figure({row, col}) != nullptr) {
                        return MoveType::False;
                    }
                }
                return MoveType::KingCastling;
            } else /*if (distance == 4)*/ {
                if (state[row][3] == true) {
                    return MoveType::False;
                }

                for (int col = 4; col < 6; ++col) {
                    if (state[row][col] == true || board.get_figure({row, col}) != nullptr) {
                        return MoveType::False;
                    }
                }
                return board.get_figure({row, 6}) == nullptr ? MoveType::QueenCastling : MoveType::False;
            }
        }
    }
    return MoveType::False;
}

void KingStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    if (row + 1 < board_rows && col + 1 < board_cols) {
        coords.emplace_back(row + 1, col + 1);
    }
    if (row + 1 < board_rows && col - 1 >= 0) {
        coords.emplace_back(row + 1, col - 1);
    }
    if (row - 1 >= 0 && col + 1 < board_cols) {
        coords.emplace_back(row - 1, col + 1);
    }
    if (row - 1 >= 0 && col - 1 >= 0) {
        coords.emplace_back(row - 1, col - 1);
    }

    if (row + 1 < board_rows) {
        coords.emplace_back(row + 1, col);
    }
    if (row - 1 >= 0) {
        coords.emplace_back(row - 1, col);
    }
    if (col + 1 < board_cols) {
        coords.emplace_back(row, col + 1);
    }
    if (col - 1 >= 0) {
        coords.emplace_back(row, col - 1);
    }
}

} // namespace Chess::Logic
