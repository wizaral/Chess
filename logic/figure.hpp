#pragma once

namespace Chess::Logic {

enum class FigureType {
    Pawn, Knight, Bishop, Rook, Queen, King,
};

enum class FigureColor {
    Light, Dark,
};

class Figure {
    FigureType type_;
    FigureColor color_;
    // field with move logic;
public:

};

};
