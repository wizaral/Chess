#pragma once
#include <array>
#include <memory>
#include <vector>

#include "definitions.hpp"
#include "figure.hpp"

namespace Chess {

class Board {
    template <class T>
    using Field = std::array<std::array<T, board_cols>, board_rows>;

    Field<std::unique_ptr<Figure>> m_figures;

    std::array<Field<bool>, 2> m_check_state;
    std::array<Field<bool>, 2> m_move_state;

public:
    Board() = default;

    const Figure *get_figure(const Position &pos) const;
    Figure *get_figure(const Position &pos);

    Position get_position(FigureType type, FigureColor color) const;
    const Field<std::unique_ptr<Figure>> &figures() const;

    bool get_check_state(const Position &pos, FigureColor color) const;
    const Field<bool> &get_check_state(FigureColor color) const;

    bool get_move_state(const Position &pos, FigureColor color) const;
    const Field<bool> &get_move_state(FigureColor color) const;

    void add_figure(Figure figure, const Position &pos);
    void move_figure(const Move &move);

    void remove_figure(const Position &pos);
    void clear_figures();

    void reset_check_state(FigureColor color);
    void update_check_state(FigureColor color, const std::vector<Position> &positions);

    void reset_move_state(FigureColor color);
    void update_move_state(FigureColor color, const std::vector<Position> &positions);

private:
    void reset(Field<bool> &field);
    void update(Field<bool> &field, const std::vector<Position> &positions);
};

} // namespace Chess
