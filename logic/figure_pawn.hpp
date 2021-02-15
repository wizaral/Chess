#include "strategy.hpp"

namespace Chess {

class Pawn final : public Figure, public Strategy, public Subscriber {
public:
    enum class MoveState {
        NoMove,
        DoubleMove,
        EnPassant,
        NormalMove,
    };

    Pawn(Publisher *publisher, FigureColor color);
    Pawn(Publisher *publisher, FigureColor color, MoveState state);
    MoveState state() const;
    bool update(MessageType type) override;

    GameState validate_move(const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    MoveState m_state = MoveState::NoMove;
    int m_direction = 0;

    GameState check_pawn(Figure *figure, FigureColor color) const;
    bool check_diagonal(const Move &move) const;
};

} // namespace Chess
