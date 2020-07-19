#include "board.hpp"
#include "strategy.hpp"

namespace Chess::Logic {

Figure *Board::get_figure(Position pos) const {
    return figures_[pos.row()][pos.col()].get();
}

Position Board::get_position(FigureType type, FigureColor color) const {
    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            if (auto f = get_figure({ i, j }); f != nullptr && f->color() == color && f->type() == type) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

const Board::Field<std::unique_ptr<Figure>> &Board::figures() const {
    return figures_;
}

bool Board::state(Position pos, FigureColor color) const {
    if (color == FigureColor::Light)
        return check_state_light_[pos.row()][pos.col()];
    return check_state_dark_[pos.row()][pos.col()];
}

const Board::Field<bool> &Board::state(FigureColor color) const {
    if (color == FigureColor::Light)
        return check_state_light_;
    return check_state_dark_;
}

void Board::add_figure(Figure figure, Position pos) {
    figures_[pos.row()][pos.col()] = std::make_unique<Figure>(std::move(figure));
}

void Board::move_figure(const Move &move) {
    figures_[move.to().row()][move.to().col()] = std::move(figures_[move.from().row()][move.from().col()]);
}

void Board::remove_figure(Position pos) {
    figures_[pos.row()][pos.col()].reset();
}

void Board::clear_figures() {
    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            remove_figure({i, j});
        }
    }
}

void Board::reset_state(FigureColor color) {
    if (color == FigureColor::Light) {
        for (auto &i : check_state_light_)
            i.fill(false);
    } else {
        for (auto &i : check_state_dark_)
            i.fill(false);
    }
}

void Board::update_state(const std::vector<Position> &positions, FigureColor color) {
    if (color == FigureColor::Light) {
        for (const auto &i : positions) {
            check_state_light_[i.row()][i.col()] = true;
        }
    } else {
        for (const auto &i : positions) {
            check_state_dark_[i.row()][i.col()] = true;
        }
    }
}

} // namespace Chess::Logic
