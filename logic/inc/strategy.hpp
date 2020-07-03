#pragma once
#include <memory>

#include "board.hpp"
#include "figure.hpp"
#include "move.hpp"
#include "observer.hpp"
#include "position.hpp"

namespace Chess::Logic {

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual bool validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) = 0;
    virtual bool update_occupation(std::unique_ptr<Figure> figure, const Board &board, const Position &pos) const = 0;
};

class PawnStrategy final : public Strategy, public Observer {
    enum class MoveState {
        NoMove, DoubleMove, NormalMove,
    } state = MoveState::NoMove;


public:
    bool validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) override;
    bool update_occupation(std::unique_ptr<Figure> figure, const Board &board, const Position &pos) const override;
};

class RookStrategy final : public Strategy {
public:
    bool validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) override;
    bool update_occupation(std::unique_ptr<Figure> figure, const Board &board, const Position &pos) const override;
};

class KnightStrategy final : public Strategy {
public:
    bool validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) override;
    bool update_occupation(std::unique_ptr<Figure> figure, const Board &board, const Position &pos) const override;
};

class BishopStrategy final : public Strategy {
public:
    bool validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) override;
    bool update_occupation(std::unique_ptr<Figure> figure, const Board &board, const Position &pos) const override;
};

class QueenStrategy final : public Strategy {
public:
    bool validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) override;
    bool update_occupation(std::unique_ptr<Figure> figure, const Board &board, const Position &pos) const override;
};

class KingStrategy final : public Strategy {
public:
    bool validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) override;
    bool update_occupation(std::unique_ptr<Figure> figure, const Board &board, const Position &pos) const override;
};

} // namespace Chess::Logic
