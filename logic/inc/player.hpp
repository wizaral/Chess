#pragma once

#include <string>

#include "figure.hpp"

namespace Chess::Logic {

class Player {
    FigureColor color_;
    std::string name_;
public:
    FigureColor color() const;
    const std::string &name() const;
};

} // namespace Chess::Logic
