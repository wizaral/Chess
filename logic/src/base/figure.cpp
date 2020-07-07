#include "figure.hpp"

namespace Chess::Logic {

Figure::Figure(FigureType type, FigureColor color, std::unique_ptr<Strategy> st)
    : type_(type), color_(color), st_(std::move(st)) {}

Figure::Figure(Figure &&f)
    : type_(f.type_), color_(f.color_), st_(std::move(f.st_)) {}

FigureType Figure::type() const {
    return type_;
}

FigureColor Figure::color() const {
    return color_;
}

Strategy *Figure::strategy() {
    return st_.get();
}

} // namespace Chess::Logic
