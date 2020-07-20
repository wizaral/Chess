#pragma once
#include <memory>
#include <vector>

#include "board.hpp"
#include "figure.hpp"
#include "gamestate.hpp"
#include "move.hpp"
#include "observer.hpp"

namespace Chess::Logic {

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual GameState validate_move(const Figure &figure, const Board &board, const Move &move) const = 0;
    virtual void move_update(const Move &move) = 0;

    virtual void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const = 0;
    virtual void update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const = 0;
};

class PawnStrategy final : public Strategy, public Subscriber {
public:
    enum class MoveState {
        NoMove,
        DoubleMove,
        EnPassant,
        NormalMove,
    };

    PawnStrategy(Publisher *publisher, FigureColor color);
    PawnStrategy(Publisher *publisher, FigureColor color, MoveState state);
    MoveState state() const;
    void update() override;

    GameState validate_move(const Figure &figure, const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    MoveState state_ = MoveState::NoMove;
    int direction_ = 0;

    GameState check_pawn(Figure *figure, FigureColor color) const;
    bool check_diagonal(const Move &move) const;
};

class RookStrategy final : public Strategy {
public:
    enum class MoveState {
        NoMove,
        NormalMove,
    };
    MoveState state() const;

    GameState validate_move(const Figure &figure, const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    MoveState state_ = MoveState::NoMove;
};

class KnightStrategy final : public Strategy {
public:
    GameState validate_move(const Figure &figure, const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const override;
};

class BishopStrategy final : public Strategy {
public:
    GameState validate_move(const Figure &figure, const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const override;
};

class QueenStrategy final : public Strategy {
public:
    GameState validate_move(const Figure &figure, const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    GameState validate(const Figure &figure, const Board &board, const Move &move, int row_inc, int col_inc) const;
    void bishop_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const;
    void rook_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const;
};

class KingStrategy final : public Strategy {
public:
    enum class MoveState {
        NoMove,
        NormalMove,
    };

    GameState validate_move(const Figure &figure, const Board &board, const Move &move) const override;
    void move_update(const Move &move) override;

    void update_occupation(const Board &board, const Position &pos, std::vector<Position> &coords) const override;
    void update_movement(const Figure &figure, const Board &board, const Position &pos, std::vector<Position> &coords) const override;

private:
    MoveState state_ = MoveState::NoMove;
    GameState check_castling(const Figure &figure, const Board &board, const Move &move, Figure *other) const;
};

} // namespace Chess::Logic
