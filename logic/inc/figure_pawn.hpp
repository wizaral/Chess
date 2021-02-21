#include "observer.hpp"
#include "figure.hpp"

namespace Chess {

class Pawn final : public Figure, public Subscriber {
public:
    enum class MoveState : uint8_t {
        NoMove,
        DoubleMove,
        EnPassant,
        NormalMove,
    };

    Pawn(FigureType type, FigureColor color, Publisher *publisher);
    Pawn(FigureType type, FigureColor color, Publisher *publisher, MoveState state);

    MoveState state() const;
    bool update(MessageType type) override;

    GameState validate_move(const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, Position pos, std::vector<Position> &coords) const override;
    void update_movement(const Board &board, Position pos, std::vector<Position> &coords) const override;

private:
    MoveState m_state = MoveState::NoMove;
    int8_t m_direction = 0;

    GameState check_pawn(const Figure *figure, FigureColor color) const;
    bool check_diagonal(const Move &move) const;
};

} // namespace Chess
