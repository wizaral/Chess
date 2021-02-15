#pragma once
#include <memory>

namespace Chess {

enum class FigureType {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
};

enum class FigureColor {
    White,
    Black,
};

FigureColor operator!(FigureColor color);

class Strategy;

class Figure final {
    FigureType m_type;
    FigureColor m_color;
    std::unique_ptr<Strategy> m_strategy;

public:
    Figure(FigureType type, FigureColor color, std::unique_ptr<Strategy> st);
    Figure(const Figure &f) = delete;
    Figure(Figure &&f);
    ~Figure() = default;

    Figure &operator=(const Figure &f) = delete;
    Figure &operator=(Figure &&f);

    FigureType type() const;
    FigureColor color() const;
    Strategy *strategy();
};

} // namespace Chess
