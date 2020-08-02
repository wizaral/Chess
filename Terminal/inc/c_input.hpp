#pragma once
#include "input.hpp"

class ConsoleInput : public Chess::DataInput {
public:
    Chess::Logic::Move get_next_move(Chess::Logic::Player *player) override;
    Chess::Logic::FigureType promote_figure(Chess::Logic::Player *player, Chess::Logic::Position pos) override;
private:
    static Chess::Logic::Position transform(int row, char col);
};
