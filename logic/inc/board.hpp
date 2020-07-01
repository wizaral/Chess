#pragma once
#include "figure.hpp"
#include <array>
#include <memory>

namespace Chess::Logic {

class Board {
    using Field = std::array<std::array<std::unique_ptr<Figure>, 8>, 8>;

    Field board;

public:
    Board() = default;

    const Figure &get_figure(Position pos) const {
        return *board[pos.row()][pos.col()].get();
    }

    void add_figure(Figure figure, Position pos) {
        Position::validation(pos);
        board[pos.row()][pos.col()] = std::make_unique<Figure>(std::move(figure));
    }

    void remove_figure(Position pos) {
        Position::validation(pos);
        board[pos.row()][pos.col()].reset();
    }

    void move_figure(const Move &move) {
        board[move.from().row()][move.from().col()] =
            std::move(board[move.to().row()][move.to().col()]);
    }
};

} // namespace Chess::Logic
