#include "board.hpp"
#include "strategy.hpp"

namespace Chess {

Figure *Board::get_figure(const Position &pos) const {
    return figures_[pos.row()][pos.col()].get();
}

Position Board::get_position(FigureType type, FigureColor color) const {
    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            if (auto f = get_figure({i, j}); f != nullptr && f->color() == color && f->type() == type) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

const Board::Field<std::unique_ptr<Figure>> &Board::figures() const {
    return figures_;
}

bool Board::get_check_state(const Position &pos, FigureColor color) const {
    return check_state_[static_cast<int>(color)][pos.row()][pos.col()];
}

const Board::Field<bool> &Board::get_check_state(FigureColor color) const {
    return check_state_[static_cast<int>(color)];
}

void Board::add_figure(Figure figure, const Position &pos) {
    figures_[pos.row()][pos.col()] = std::make_unique<Figure>(std::move(figure));
}

void Board::move_figure(const Move &move) {
    figures_[move.to().row()][move.to().col()] = std::move(figures_[move.from().row()][move.from().col()]);
}

void Board::remove_figure(const Position &pos) {
    figures_[pos.row()][pos.col()].reset();
}

void Board::clear_figures() {
    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            remove_figure({i, j});
        }
    }
}

void Board::reset_check_state(FigureColor color) {
    reset(check_state_[static_cast<int>(color)]);
}

void Board::update_check_state(FigureColor color, const std::vector<Position> &positions) {
    update(check_state_[static_cast<int>(color)], positions);
}

void Board::reset_move_state(FigureColor color) {
    reset(move_state_[static_cast<int>(color)]);
}

void Board::update_move_state(FigureColor color, const std::vector<Position> &positions) {
    update(move_state_[static_cast<int>(color)], positions);
}

void Board::reset(Field<bool> &field) {
    for (auto &i : field) {
        i.fill(false);
    }
}

void Board::update(Field<bool> &field, const std::vector<Position> &positions) {
    for (const auto &i : positions) {
        field[i.row()][i.col()] = true;
    }
}

} // namespace Chess
