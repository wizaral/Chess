#pragma once
#include <memory>

#include "board.hpp"
#include "figure.hpp"
#include "move.hpp"

namespace Chess::Logic {

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) const = 0;
};

class BishopStrategy : public Strategy {
public:
    void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) const override;
};

class KingStrategy : public Strategy {
public:
    void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) const override;
};

class KnightStrategy : public Strategy {
public:
    void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) const override;
};

class PawnStrategy : public Strategy {
public:
    void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) const override;
};

class RookStrategy : public Strategy {
public:
    void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) const override;
};

class QueenStrategy : public BishopStrategy, public RookStrategy {
public:
    void validate_move(std::unique_ptr<Figure> figure, const Board &board, const Move &move) const override;
};

}; // namespace Chess::Logic
