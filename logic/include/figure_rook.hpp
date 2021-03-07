#include "figure.hpp"

namespace Chess {

class Rook final : public Figure {
public:
    enum class MoveState : uint8_t {
        NoMove,
        NormalMove,
    };

    using Figure::Figure;

    Rook(FigureType type, FigureColor color, MoveState state);
    MoveState state() const;

    GameState validate_move(const Board &board, Move move) const override;
    void move_update(Move move) override;

    void update_occupation(const Board &board, Position pos, std::vector<Position> &coords) const override;
    void update_movement(const Board &board, Position pos, std::vector<Position> &coords) const override;

private:
    MoveState m_state = MoveState::NoMove;
};

} // namespace Chess
