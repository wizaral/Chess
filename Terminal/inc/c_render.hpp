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
    void show_next_step(Chess::Logic::Player *player) override;
    void show_error(Chess::Logic::GameState state) override;
    void show_pawn_promotion(Chess::Logic::Position pos) override;
    void show_endgame(Chess::Logic::GameState state, Chess::Logic::Player *winer) override;

private:
    inline static const std::map<Chess::Logic::FigureType, char> figures{
        {Chess::Logic::FigureType::Bishop, 'B'},
        {Chess::Logic::FigureType::King, 'K'},
        {Chess::Logic::FigureType::Knight, 'N'},
        {Chess::Logic::FigureType::Pawn, 'P'},
        {Chess::Logic::FigureType::Queen, 'Q'},
        {Chess::Logic::FigureType::Rook, 'R'},
    };

    inline static const std::map<Chess::Logic::FigureColor, char> colors{
        {Chess::Logic::FigureColor::White, 'w'},
        {Chess::Logic::FigureColor::Black, 'b'},
    };

    inline const static std::map<Chess::Logic::GameState, std::string> states{
        {Chess::Logic::GameState::CastlingFigureOnPath, "CastlingFigureOnPath"},
        {Chess::Logic::GameState::CastlingKingInCheck, "CastlingKingInCheck"},
        {Chess::Logic::GameState::CastlingSquareInCheck, "CastlingSquareInCheck"},
        {Chess::Logic::GameState::CheckMate, "CheckMate"},
        {Chess::Logic::GameState::Draw, "Draw"},
        {Chess::Logic::GameState::EmptySquare, "EmptySquare"},
        {Chess::Logic::GameState::FailEnPassant, "FailEnPassant"},
        {Chess::Logic::GameState::FiguresAlreadyMoved, "FiguresAlreadyMoved"},
        {Chess::Logic::GameState::KingCastling, "KingCastling"},
        {Chess::Logic::GameState::KingInCheck, "KingInCheck"},
        {Chess::Logic::GameState::KingWillBeInCheck, "KingWillBeInCheck"},
        {Chess::Logic::GameState::NormalMove, "NormalMove"},
        {Chess::Logic::GameState::OtherFigureOnPath, "OtherFigureOnPath"},
        {Chess::Logic::GameState::OutOfBounds, "OutOfBounds"},
        {Chess::Logic::GameState::PawnStepBack, "PawnStepBack"},
        {Chess::Logic::GameState::QueenCastling, "QueenCastling"},
        {Chess::Logic::GameState::StaleMate, "StaleMate"},
        {Chess::Logic::GameState::WrongFigureColor, "WrongFigureColor"},
        {Chess::Logic::GameState::WrongFigureMove, "WrongFigureMove"},
    };
};
