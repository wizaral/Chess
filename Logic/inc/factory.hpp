#pragma once
#include "board.hpp"
#include "observer.hpp"
#include "player.hpp"

namespace Chess {

class FigureFactory {
public:
    virtual ~FigureFactory() = default;
    virtual void init_game(Board &board, int &player_index, Publisher *publisher) = 0;
};

class ClassicFactory final : public FigureFactory {
public:
    void init_game(Board &board, int &player_index, Publisher *publisher) override;

private:
    void spawn_pawns(Board &board, int row, FigureColor color, Publisher *publisher);
    void spawn_figures(Board &board, int row, FigureColor color);
};

} // namespace Chess
