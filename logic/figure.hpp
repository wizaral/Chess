#pragma once
#include <vector>

#include "gamestate.hpp"
#include "move.hpp"
#include "observer.hpp"

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
    uint8_t m_color_type;

public:
    Figure(FigureType type, FigureColor color);
    virtual ~Figure() = default;

    FigureType type() const;
    FigureColor color() const;

    virtual GameState validate_move(const Board &board, const Move &move) const = 0;
    virtual void move_update(const Move &move) = 0;

    virtual void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const = 0;
    virtual void update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const = 0;
};

} // namespace Chess
