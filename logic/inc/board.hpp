#pragma once
#include <array>
#include <memory>
#include <vector>

#include "definitions.hpp"
#include "figure.hpp"
#include "move.hpp"

namespace Chess::Logic {

class Board {
    template <class T>
    using Field = std::array<std::array<T, board_cols>, board_rows>;

    Field<std::unique_ptr<Figure>> figures_;
    Field<bool> check_state_white_;
    Field<bool> check_state_black_;

public:
    Board() = default;

    Figure *get_figure(Position pos) const;
    Position get_position(FigureType type, FigureColor color) const;
    const Field<std::unique_ptr<Figure>> &figures() const;

    bool state(Position pos, FigureColor color) const;
    const Field<bool> &state(FigureColor color) const;

    void add_figure(Figure figure, Position pos);
    void move_figure(const Move &move);

    void remove_figure(Position pos);
    void clear_figures();

    void reset_state(FigureColor color);
    void update_state(const std::vector<Position> &positions, FigureColor color);
};

} // namespace Chess::Logic
