#pragma once
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

#include "render.hpp"

class ConsoleRender : public Chess::Render {
public:
    void show_menu() override;
    void show_board(const Chess::Logic::Board &board) override;
    void show_error(Chess::Logic::GameState state) override;
    void show_pawn_promotion(Chess::Logic::Position pos) override;
    void show_endgame(Chess::Logic::GameState state, Chess::Logic::Player *winer) override;
private:
    const std::map<Chess::Logic::FigureType, char> figures{
        {Chess::Logic::FigureType::Bishop, 'B'},
        {Chess::Logic::FigureType::King, 'K'},
        {Chess::Logic::FigureType::Knight, 'N'},
        {Chess::Logic::FigureType::Pawn, 'P'},
        {Chess::Logic::FigureType::Queen, 'Q'},
        {Chess::Logic::FigureType::Rook, 'R'},
    };
    const std::map<Chess::Logic::FigureColor, char> colors{
        {Chess::Logic::FigureColor::Light, 'W'},
        {Chess::Logic::FigureColor::Dark, 'D'},
    };
};
