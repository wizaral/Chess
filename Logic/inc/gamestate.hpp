#pragma once

namespace Chess {

enum class GameState {
    NormalMove,
    EnPassant,
    KingCastling,
    QueenCastling,
    PawnPromotion,

    CheckMate,
    StaleMate,
    Draw,

    OutOfBounds,
    EmptySquare,
    WrongFigureColor,
    WrongFigureMove,
    OtherFigureOnPath,

    KingInCheck,
    KingWillBeInCheck,

    FiguresAlreadyMoved,
    CastlingKingInCheck,
    CastlingSquareInCheck,
    CastlingFigureOnPath,

    FailEnPassant,
    PawnStepBack,
    SIZE,
};

bool is_move(GameState gstate);
bool is_endgame(GameState gstate);
bool is_error(GameState gstate);

} // namespace Chess
