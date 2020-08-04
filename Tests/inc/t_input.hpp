#pragma once
#include "input.hpp"

class TestInput : public Chess::DataInput {
public:
    Chess::Move get_next_move(Chess::Player *player) override;
    Chess::FigureType promote_figure(Chess::Player *player, Chess::Position pos) override;
    static Chess::Position transform(int row, char col);
};
