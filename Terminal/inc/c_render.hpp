#pragma once
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

#include "render.hpp"

class ConsoleRender : public Chess::Render {
public:
    void show_menu() override;
    void show_board(const Chess::Board &board) override;
    void show_next_step(Chess::Player *player) override;
    void show_error(Chess::GameState state) override;
    void show_pawn_promotion(Chess::Position pos) override;
    void show_endgame(Chess::GameState state, Chess::Player *winer) override;

private:
    inline static const std::map<Chess::FigureType, char> figures{
        {Chess::FigureType::Bishop, 'B'},
        {Chess::FigureType::King, 'K'},
        {Chess::FigureType::Knight, 'N'},
        {Chess::FigureType::Pawn, 'P'},
        {Chess::FigureType::Queen, 'Q'},
        {Chess::FigureType::Rook, 'R'},
    };

    inline static const std::map<Chess::FigureColor, char> colors{
        {Chess::FigureColor::White, 'w'},
        {Chess::FigureColor::Black, 'b'},
    };

    inline const static std::map<Chess::GameState, std::string> states{
        {Chess::GameState::CastlingFigureOnPath, "CastlingFigureOnPath"},
        {Chess::GameState::CastlingKingInCheck, "CastlingKingInCheck"},
        {Chess::GameState::CastlingSquareInCheck, "CastlingSquareInCheck"},
        {Chess::GameState::CheckMate, "CheckMate"},
        {Chess::GameState::Draw, "Draw"},
        {Chess::GameState::EmptySquare, "EmptySquare"},
        {Chess::GameState::FailEnPassant, "FailEnPassant"},
        {Chess::GameState::FiguresAlreadyMoved, "FiguresAlreadyMoved"},
        {Chess::GameState::KingCastling, "KingCastling"},
        {Chess::GameState::KingInCheck, "KingInCheck"},
        {Chess::GameState::KingWillBeInCheck, "KingWillBeInCheck"},
        {Chess::GameState::NormalMove, "NormalMove"},
        {Chess::GameState::OtherFigureOnPath, "OtherFigureOnPath"},
        {Chess::GameState::OutOfBounds, "OutOfBounds"},
        {Chess::GameState::PawnStepBack, "PawnStepBack"},
        {Chess::GameState::QueenCastling, "QueenCastling"},
        {Chess::GameState::StaleMate, "StaleMate"},
        {Chess::GameState::WrongFigureColor, "WrongFigureColor"},
        {Chess::GameState::WrongFigureMove, "WrongFigureMove"},
    };
};
