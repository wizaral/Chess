#include "board.hpp"

namespace Chess::Logic {

Board::Board() = default;

Figure *Board::get_figure(Position pos) const {
    return figures_[pos.row()][pos.col()].get();
}

const Board::Field<std::unique_ptr<Figure>> &Board::figures() const {
    return figures_;
}

bool Board::state(Position pos, FigureColor color) const {
    if (color == FigureColor::Light)
        return occupate_state_light_[pos.row()][pos.col()];
    return occupate_state_dark_[pos.row()][pos.col()];
}

const Board::Field<bool> &Board::state(FigureColor color) const {
    if (color == FigureColor::Light)
        return occupate_state_light_;
    return occupate_state_dark_;
}

void Board::add_figure(Figure figure, Position pos) {
    Position::validation(pos);
    figures_[pos.row()][pos.col()] = std::make_unique<Figure>(std::move(figure));
}

void Board::remove_figure(Position pos) {
    Position::validation(pos);
    figures_[pos.row()][pos.col()].reset();
}

void Board::move_figure(const Move &move) {
    figures_[move.from().row()][move.from().col()] =
        std::move(figures_[move.to().row()][move.to().col()]);
}

void Board::reset_state() {
    for (auto &i : occupate_state_light_)
        i.fill(false);
    for (auto &i : occupate_state_dark_)
        i.fill(false);
}

void Board::update_state(const std::vector<Position> &positions, FigureColor color) {
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

} // namespace Chess::Logic
