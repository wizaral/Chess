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
    Field<bool> check_state_light_;
    Field<bool> check_state_dark_;

public:
    Board();

    Figure *get_figure(Position pos) const;
    const Field<std::unique_ptr<Figure>> &figures() const;

    bool state(Position pos, FigureColor color) const;
    const Field<bool> &state(FigureColor color) const;

    void add_figure(Figure figure, Position pos);
    void remove_figure(Position pos);
    void move_figure(const Move &move);

    void reset_state();
    void update_state(const std::vector<Position> &positions, FigureColor color);
};

} // namespace Chess::Logic
