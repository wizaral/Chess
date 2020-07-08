#pragma once
#include <memory>

#include "board.hpp"
#include "definitions.hpp"
#include "figure.hpp"
#include "move.hpp"
#include "observer.hpp"
#include "position.hpp"

namespace Chess::Logic {

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual bool validate_move(const Figure &figure, const Board &board, const Move &move) = 0;
    virtual bool update_occupation(const Figure &figure, const Board &board, const Position &pos) const = 0;
};

class PawnStrategy final : public Strategy, public Subscriber {
public:
    enum class MoveState {
        NoMove, DoubleMove, NormalMove,
    };

    PawnStrategy(Publisher *subscriber, int direction);
    MoveState state() const;
    void update() override;

    bool validate_move(const Figure &figure, const Board &board, const Move &move) override;
    bool update_occupation(const Figure &figure, const Board &board, const Position &pos) const override;
private:
    MoveState state_ = MoveState::NoMove;
    int direction_ = 0;

    bool check_pawn(Figure *figure, FigureColor color);
    bool check_diagonal(const Move &move);
};

class RookStrategy final : public Strategy {
public:
    enum class MoveState {
        NoMove, NormalMove,
    };

    bool validate_move(const Figure &figure, const Board &board, const Move &move) override;
    bool update_occupation(const Figure &figure, const Board &board, const Position &pos) const override;
private:
    MoveState state_ = MoveState::NoMove;
};

class KnightStrategy final : public Strategy {
public:
    bool validate_move(const Figure &figure, const Board &board, const Move &move) override;
    bool update_occupation(const Figure &figure, const Board &board, const Position &pos) const override;
};

class BishopStrategy final : public Strategy {
public:
    bool validate_move(const Figure &figure, const Board &board, const Move &move) override;
    bool update_occupation(const Figure &figure, const Board &board, const Position &pos) const override;
};

class QueenStrategy final : public Strategy {
public:
    bool validate_move(const Figure &figure, const Board &board, const Move &move) override;
    bool update_occupation(const Figure &figure, const Board &board, const Position &pos) const override;
private:
    bool validate(const Figure &figure, const Board &board, const Move &move, int row_inc, int col_inc);
};

class KingStrategy final : public Strategy {
public:
    enum class MoveState {
        NoMove, NormalMove,
    };

    bool validate_move(const Figure &figure, const Board &board, const Move &move) override;
    bool update_occupation(const Figure &figure, const Board &board, const Position &pos) const override;
private:
    MoveState state_ = MoveState::NoMove;
};

} // namespace Chess::Logic
