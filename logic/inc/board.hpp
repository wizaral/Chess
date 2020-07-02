#pragma once
#include "figure.hpp"
#include <array>
#include <memory>
#include <utility>

namespace Chess::Logic {

class Board {
    template <class T>
    using Field = std::array<std::array<T, 8>, 8>;

    Field<std::unique_ptr<Figure>> figures_;
    Field<std::pair<bool, bool>> check_state_;

public:
    Board() = default;

    Figure *get_figure(Position pos) const {
        return figures_[pos.row()][pos.col()].get();
    }

    bool get_state(Position pos, FigureColor color) const {
        if (color == FigureColor::Light)
            return check_state_[pos.row()][pos.col()].first;
        return check_state_[pos.row()][pos.col()].second;
    }

    void add_figure(Figure figure, Position pos) {
        Position::validation(pos);
        figures_[pos.row()][pos.col()] = std::make_unique<Figure>(std::move(figure));
    }

    void remove_figure(Position pos) {
        Position::validation(pos);
        figures_[pos.row()][pos.col()].reset();
    }

    void move_figure(const Move &move) {
        figures_[move.from().row()][move.from().col()] =
            std::move(figures_[move.to().row()][move.to().col()]);
    }
};

} // namespace Chess::Logic
