#pragma once

namespace Chess {

#define FigureTypeMask 0x3F
#define FigureColorMask 0xC0

enum class FigureType {
    Pawn = 1 << 0,
    Knight = 1 << 1,
    Bishop = 1 << 2,
    Rook = 1 << 3,
    Queen = 1 << 4,
    King = 1 << 5,
};

enum class FigureColor {
    White = 1 << 6,
    Black = 1 << 7,
};

FigureColor operator!(FigureColor color);

class Figure {
protected:
    int8_t m_color_type;

public:
    Figure(FigureType type, FigureColor color);
    virtual ~Figure() = default;

    FigureType type() const;
    FigureColor color() const;
};

} // namespace Chess
