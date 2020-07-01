#pragma once
#include <stdexcept>

namespace Chess::Logic {

class Position {
    int row_, col_;
public:
    Position(int row, int col) : row_(row), col_(col) {}

    int row() const {
        return row_;
    }
    int col() const {
        return col_;
    }

    static void validation(const Position &pos) {
        if (int row = pos.row(); row < 0 || row > 7) {
            throw std::out_of_range("Row position isn't valid");
        }
        if (int col = pos.col(); col < 0 || col > 7) {
            throw std::out_of_range("Col position isn't valid");
        }
    }
};

} // namespace Chess::Logic
