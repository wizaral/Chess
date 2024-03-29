#pragma once
#include <vector>

#include "gamestate.hpp"
#include "move.hpp"

namespace Chess {

enum class FigureType : uint8_t {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
};

enum class FigureColor : uint8_t {
    White,
    Black,
};

FigureColor operator!(FigureColor color);

class Board;

class Figure {
protected:
    FigureType m_type;
    FigureColor m_color;

public:
    Figure(FigureType type, FigureColor color);
    virtual ~Figure() = default;

    FigureType type() const;
    FigureColor color() const;

    virtual GameState validate_move(const Board &board, Move move) const = 0;
    virtual void move_update(Move move) = 0;

    virtual void update_occupation(const Board &board, Position pos, std::vector<Position> &coords) const = 0;
    virtual void update_movement(const Board &board, Position pos, std::vector<Position> &coords) const = 0;
};

} // namespace Chess
