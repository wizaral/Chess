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

class Figure {
    FigureType type_;
    FigureColor color_;
    std::unique_ptr<Strategy> st_;
public:
    Figure(FigureType type, FigureColor color, std::unique_ptr<Strategy> st)
        : type_(type), color_(color), st_(std::move(st)) {}

    Figure(Figure &&f)
        : type_(f.type_), color_(f.color_), st_(std::move(f.st_)) {}

    ~Figure() = default;

    FigureType type() const {
        return type_;
    }

    FigureColor color() const {
        return color_;
    }

    Strategy *strategy() {
        return st_.get();
    }
};

} // namespace Chess::Logic
