#pragma once
#include <memory>

#include "strategy.hpp"

namespace Chess::Logic {

enum class FigureType {
    Pawn, Knight, Bishop, Rook, Queen, King,
};

enum class FigureColor {
    Light, Dark,
};

class Figure final {
    FigureType type_;
    FigureColor color_;
    std::unique_ptr<Strategy> st_;
public:
    Figure(FigureType type, FigureColor color, std::unique_ptr<Strategy> st);
    Figure(Figure &&f);
    ~Figure() = default;
    Figure &operator=(Figure&& f);

    FigureType type() const;
    FigureColor color() const;
    Strategy *strategy();
};

} // namespace Chess::Logic
