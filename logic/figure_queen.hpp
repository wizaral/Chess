#include "figure.hpp"

namespace Chess {

class Queen final : public Figure {
public:
    using Figure::Figure;

    GameState validate_move(const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    GameState validate(const Board &board, const Move &move, int row_inc, int col_inc) const;
    void bishop_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const;
    void rook_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const;
};

} // namespace Chess
