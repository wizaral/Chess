#include "figure.hpp"
#include "strategy.hpp"

namespace Chess {

FigureColor operator!(FigureColor color) {
    return static_cast<FigureColor>(static_cast<int>(color) ^ 1);
}

Figure::Figure(FigureType type, FigureColor color, std::unique_ptr<Strategy> st)
: m_type(type)
, m_color(color)
, m_strategy(std::move(st)) {}

Figure::Figure(Figure &&f)
: m_type(f.m_type)
, m_color(f.m_color)
, m_strategy(std::move(f.m_strategy)) {}

Figure &Figure::operator=(Figure &&f) {
    if (this != &f) {
        m_type = f.m_type;
        m_color = f.m_color;
        m_strategy = std::move(f.m_strategy);
    }
    return *this;
}

FigureType Figure::type() const {
    return m_type;
}

FigureColor Figure::color() const {
    return m_color;
}

Strategy *Figure::strategy() {
    return m_strategy.get();
}

} // namespace Chess
