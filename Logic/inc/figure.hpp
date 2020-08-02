#pragma once
#include <memory>

namespace Chess::Logic {

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
    FigureType type_;
    FigureColor color_;
    std::unique_ptr<Strategy> st_;

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

} // namespace Chess::Logic
