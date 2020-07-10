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

inline bool is_endgame(GameState gstate) {
    return gstate == GameState::CheckMate
        || gstate == GameState::StaleMate
        || gstate == GameState::Draw;
}

inline bool is_error(GameState gstate) {
    return gstate != GameState::NormalMove
        && gstate != GameState::KingCastling
        && gstate != GameState::QueenCastling
        && gstate != GameState::CheckMate
        && gstate != GameState::Draw;
}

} // namespace Chess::Logic
