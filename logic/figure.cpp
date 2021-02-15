#include "figure.hpp"
#include "strategy.hpp"

namespace Chess {

FigureColor operator!(FigureColor color) {
    return static_cast<FigureColor>(static_cast<int8_t>(color) ^ FigureColorMask);
}

Figure::Figure(FigureType type, FigureColor color)
: m_color_type(static_cast<int8_t>(type) | static_cast<int8_t>(color)) {}

FigureType Figure::type() const {
    return static_cast<FigureType>(m_color_type & FigureTypeMask);
}

FigureColor Figure::color() const {
    return static_cast<FigureColor>(m_color_type & FigureColorMask);
}

} // namespace Chess
