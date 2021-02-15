#include "strategy.hpp"

namespace Chess {

class Rook final : public Figure, public Strategy {
public:
    enum class MoveState {
        NoMove,
        NormalMove,
    };
    MoveState state() const;

    GameState validate_move(const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    MoveState m_state = MoveState::NoMove;
};

} // namespace Chess
