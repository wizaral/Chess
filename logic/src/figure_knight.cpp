#include "board.hpp"
#include "figure_knight.hpp"

namespace Chess {

GameState Knight::validate_move(const Board &board, Move move) const {
    int rows = move.rows();
    int cols = move.cols();

    if ((rows == 2 && cols == 1) || (rows == 1 && cols == 2)) {
        auto other = board.get_figure(move.to());

        if (other == nullptr || other->color() != color()) {
            return GameState::NormalMove;
        } else {
            return GameState::OtherFigureOnPath;
        }
    }
    return GameState::WrongFigureMove;
}

void Knight::update_occupation(const Board &board, Position pos, std::vector<Position> &coords) const {
    int row = pos.row(), col = pos.col();

    std::array<Position, 8> positions{
        Position{row + 2, col + 1},
        Position{row + 2, col - 1},
        Position{row + 1, col + 2},
        Position{row - 1, col + 2},
        Position{row - 2, col + 1},
        Position{row - 2, col - 1},
        Position{row + 1, col - 2},
        Position{row - 1, col - 2},
    };

    for (auto p : positions) {
        if (Position::validation(p)) {
            coords.push_back(p);
        }
    }
}

void Knight::update_movement(const Board &board, Position pos, std::vector<Position> &coords) const {
    update_occupation(board, pos, coords);
}

void Knight::move_update(Move move) {}

} // namespace Chess
