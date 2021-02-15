#pragma once

namespace Chess {

#define FigureTypeMask 0x3F
#define FigureColorMask 0xC0

enum class FigureType : uint8_t {
    Pawn = 1 << 0,
    Knight = 1 << 1,
    Bishop = 1 << 2,
    Rook = 1 << 3,
    Queen = 1 << 4,
    King = 1 << 5,
};

enum class FigureColor : uint8_t {
    White = 1 << 6,
    Black = 1 << 7,
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
