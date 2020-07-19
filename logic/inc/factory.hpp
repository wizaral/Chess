#pragma once
#include "board.hpp"
#include "observer.hpp"
#include "player.hpp"

namespace Chess {

class FigureFactory {
public:
    virtual ~FigureFactory() = default;
    virtual void init_game(Logic::Board &board, int &player_index, std::array<std::unique_ptr<Logic::Player>, 2> &players, Publisher *publisher) = 0;
};

class ClassicFactory final : public FigureFactory {
public:
    void init_game(Logic::Board &board, int &player_index, std::array<std::unique_ptr<Logic::Player>, 2> &players, Publisher *publisher) override;
private:
    void spawn_pawns(Logic::Board &board, int row, Logic::FigureColor color, int direction, Publisher *publisher);
    void spawn_figures(Logic::Board &board, int row, Logic::FigureColor color);
};

} // namespace Chess
