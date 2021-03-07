#include "figure.hpp"

namespace Chess {

class Knight final : public Figure {
public:
    using Figure::Figure;

    GameState validate_move(const Board &board, Move move) const override;
    void move_update(Move move) override;

    void update_occupation(const Board &board, Position pos, std::vector<Position> &coords) const override;
    void update_movement(const Board &board, Position pos, std::vector<Position> &coords) const override;
};

} // namespace Chess
