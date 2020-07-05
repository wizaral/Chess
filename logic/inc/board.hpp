#pragma once
#include <array>
#include <memory>
#include <vector>

#include "figure.hpp"

namespace Chess::Logic {

class Board {
    template <class T>
    using Field = std::array<std::array<T, 8>, 8>;

    Field<std::unique_ptr<Figure>> figures_;
    Field<bool> occupate_state_light_;
    Field<bool> occupate_state_dark_;

public:
    Board() = default;

    Figure *get_figure(Position pos) const {
        return figures_[pos.row()][pos.col()].get();
    }

    const Field<std::unique_ptr<Figure>> &figures() const {
        return figures_;
    }

    bool state(Position pos, FigureColor color) const {
        if (color == FigureColor::Light)
            return occupate_state_light_[pos.row()][pos.col()];
        return occupate_state_dark_[pos.row()][pos.col()];
    }

    const Field<bool> &state(FigureColor color) const {
        if (color == FigureColor::Light)
            return occupate_state_light_;
        return occupate_state_dark_;
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

    void reset_state() {
        for (auto &i : occupate_state_light_)
            i.fill(false);
        for (auto &i : occupate_state_dark_)
            i.fill(false);
    }

    void update_state(const std::vector<Position> &positions, FigureColor color) {
        if (color == FigureColor::Light) {
            for (const auto &i : positions) {
                Position::validation(i);
                occupate_state_light_[i.row()][i.col()] = true;
            }
        } else {
            for (const auto &i : positions) {
                Position::validation(i);
                occupate_state_dark_[i.row()][i.col()] = true;
            }
        }
    }
};

} // namespace Chess::Logic
