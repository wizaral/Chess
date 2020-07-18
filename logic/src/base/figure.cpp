#include "figure.hpp"
#include "strategy.hpp"

namespace Chess::Logic {

FigureColor operator!(FigureColor color) {
    return color == Logic::FigureColor::Light ? Logic::FigureColor::Dark : Logic::FigureColor::Light;
}

Figure::Figure(FigureType type, FigureColor color, std::unique_ptr<Strategy> st)
    : type_(type), color_(color), st_(std::move(st)) {}

Figure::Figure(Figure &&f)
    : type_(f.type_), color_(f.color_), st_(std::move(f.st_)) {}

Figure &Figure::operator=(Figure&& f) {
    if (this != &f) {
        type_ = f.type_;
        color_ = f.color_;
        st_ = std::move(f.st_);
    }
    return *this;
}

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
