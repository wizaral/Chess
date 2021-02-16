#include "figure.hpp"

namespace Chess {

FigureColor operator!(FigureColor color) {
    return static_cast<FigureColor>(static_cast<uint8_t>(color) ^ 1);
}

Figure::Figure(FigureType type, FigureColor color)
: m_type(type)
, m_color(color) {}

FigureType Figure::type() const {
    return m_type;
}

FigureColor Figure::color() const {
    return m_color;
}

} // namespace Chess
