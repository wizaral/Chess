#pragma once

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

class Figure {
protected:
    uint8_t m_color_type;

public:
    Figure(FigureType type, FigureColor color);
    virtual ~Figure() = default;

    FigureType type() const;
    FigureColor color() const;
};

} // namespace Chess
