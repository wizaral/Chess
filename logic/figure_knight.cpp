#include "figure_knight.hpp"

namespace Chess {

GameState Knight::validate_move(const Board &board, const Move &move) const {
    int rows = move.rows();
    int cols = move.cols();

    if ((rows == 2 && cols == 1) || (rows == 1 && cols == 2)) {
        Figure *other = board.get_figure(move.to());

        if (other == nullptr || other->color() != color()) {
            return GameState::NormalMove;
        } else {
            return GameState::OtherFigureOnPath;
        }
    }
    return GameState::WrongFigureMove;
}

void Knight::update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    if (Position::validation({row + 2, col + 1})) {
        coords.emplace_back(row + 2, col + 1);
    }
    if (Position::validation({row + 2, col - 1})) {
        coords.emplace_back(row + 2, col - 1);
    }
    if (Position::validation({row + 1, col + 2})) {
        coords.emplace_back(row + 1, col + 2);
    }
    if (Position::validation({row - 1, col + 2})) {
        coords.emplace_back(row - 1, col + 2);
    }
    if (Position::validation({row - 2, col + 1})) {
        coords.emplace_back(row - 2, col + 1);
    }
    if (Position::validation({row - 2, col - 1})) {
        coords.emplace_back(row - 2, col - 1);
    }
    if (Position::validation({row + 1, col - 2})) {
        coords.emplace_back(row + 1, col - 2);
    }
    if (Position::validation({row - 1, col - 2})) {
        coords.emplace_back(row - 1, col - 2);
    }
}

void Knight::update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void Knight::move_update(const Move &move) {}

} // namespace Chess
