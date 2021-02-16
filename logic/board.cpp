#include "board.hpp"

namespace Chess {

const Figure *Board::get_figure(const Position &pos) const {
    return m_figures[pos.row()][pos.col()].get();
}

Figure *Board::get_figure(const Position &pos) {
    return m_figures[pos.row()][pos.col()].get();
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
    return m_figures;
}

bool Board::get_check_state(const Position &pos, FigureColor color) const {
    return m_check_state[static_cast<int>(color)][pos.row()][pos.col()];
}

const Board::Field<bool> &Board::get_check_state(FigureColor color) const {
    return m_check_state[static_cast<int>(color)];
}

void Board::add_figure(std::unique_ptr<Figure> figure, const Position &pos) {
    m_figures[pos.row()][pos.col()] = std::move(figure);
}

std::unique_ptr<Figure> Board::extract_figure(const Position &pos) {
    return std::move(m_figures[pos.row()][pos.col()]);
}

void Board::move_figure(const Move &move) {
    m_figures[move.to().row()][move.to().col()] = std::move(m_figures[move.from().row()][move.from().col()]);
}

void Board::remove_figure(const Position &pos) {
    m_figures[pos.row()][pos.col()].reset();
}

void Board::clear_figures() {
    for (int i = 0; i < board_rows; ++i) {
        for (int j = 0; j < board_cols; ++j) {
            remove_figure({i, j});
        }
    }
}

void Board::reset_check_state(FigureColor color) {
    reset(m_check_state[static_cast<int>(color)]);
}

void Board::update_check_state(FigureColor color, const std::vector<Position> &positions) {
    update(m_check_state[static_cast<int>(color)], positions);
}

void Board::reset_move_state(FigureColor color) {
    reset(m_move_state[static_cast<int>(color)]);
}

void Board::update_move_state(FigureColor color, const std::vector<Position> &positions) {
    update(m_move_state[static_cast<int>(color)], positions);
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
