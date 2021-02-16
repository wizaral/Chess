#include "figure.hpp"

namespace Chess {

class King final : public Figure {
public:
    enum class MoveState : uint8_t {
        NoMove,
        NormalMove,
    };

    using Figure::Figure;

    King(FigureType type, FigureColor color, MoveState state);
    MoveState state() const;

    GameState validate_move(const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    MoveState m_state = MoveState::NoMove;

    GameState check_castling(const Board &board, const Move &move, const Figure *other) const;
};

} // namespace Chess
