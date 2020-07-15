#pragma once

namespace Chess::Logic {

enum class GameState {
    NormalMove,
    KingCastling,
    QueenCastling,

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
};

bool is_move(GameState gstate);
bool is_endgame(GameState gstate);
bool is_error(GameState gstate);

} // namespace Chess::Logic
