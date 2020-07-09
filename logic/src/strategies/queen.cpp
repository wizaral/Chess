#include "strategy.hpp"

namespace Chess::Logic {

MoveType QueenStrategy::validate_move(const Figure &figure, const Board &board, const Move &move) {
    int rows = move.rows();
    int cols = move.cols();

    if (rows == 0 && cols == 0)
        return MoveType::False; // WrongFigureMove

    if (rows == cols) {
        int row_inc = move.from().row() < move.to().row() ? 1 : -1;
        int col_inc = move.from().col() < move.to().col() ? 1 : -1;
        return validate(figure, board, move, row_inc, col_inc);
    }
    if ((rows > 0 && cols == 0) || (rows == 0 && cols > 0)) {
        int row_inc = rows != 0 ? (move.from().row() < move.to().row() ? 1 : -1) : 0;
        int col_inc = cols != 0 ? (move.from().col() < move.to().col() ? 1 : -1) : 0;
        return validate(figure, board, move, row_inc, col_inc);
    }

    return MoveType::False; // WrongFigureMove
}

MoveType QueenStrategy::validate(const Figure &figure, const Board &board, const Move &move, int row_inc, int col_inc) {
    int pos_row = move.from().row();
    int pos_col = move.from().col();

    while (true) {
        pos_row += row_inc;
        pos_col += col_inc;

        if (move.to().row() == pos_row && move.to().col() == pos_col) {
            Figure *other = board.get_figure(move.to());

            if (other != nullptr && other->color() == figure.color())
                return MoveType::False; // OtherFigureOnPath
            return MoveType::True;  // NormalMove
        }

        if (board.get_figure({pos_row, pos_col}) != nullptr)
            return MoveType::False; // OtherFigureOnPath
    }
}

void QueenStrategy::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    bishop_occupation(board, pos, coords);
    rook_occupation(board, pos, coords);
}

void QueenStrategy::bishop_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    for (int r = row + 1, c = col + 1; r < board_rows && c < board_cols; ++r, ++c) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row - 1, c = col + 1; r >= 0 && c < board_cols; --r, ++c) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row + 1, c = col - 1; r < board_rows && c >= 0; ++r, --c) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }
}

void QueenStrategy::rook_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    for (int r = row + 1, c = col; r < board_rows; ++r) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row - 1, c = col; r >= 0; --r) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row, c = col + 1; c < board_cols; ++c) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }

    for (int r = row, c = col - 1; c >= 0; --c) {
        coords.emplace_back(r, c);

        if (board.get_figure({r, c}) != nullptr) {
            break;
        }
    }
}

} // namespace Chess::Logic
