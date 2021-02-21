#pragma once
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <stdexcept>

#include "factory.hpp"

namespace Chess {

class Logic : public Publisher {
    Board m_board;
    std::array<std::unique_ptr<Player>, players_amount> m_players;

    int m_player_index = 0;
    Position m_pawn_pos = Position::invalid;
    GameState m_state = GameState::NormalMove;

public:
    Logic(std::array<std::unique_ptr<Player>, players_amount> arr);

    Player *player();
    const Board &board() const;

    void init_game(std::unique_ptr<FigureFactory> factory);

    GameState logic(const Move &move);
    GameState promote_pawn(FigureType type);

    std::vector<Position> possible_moves(Position pos);
    std::vector<Position> possible_moves(Position pos, FigureColor color);

private:
    void validate_players() const;
    GameState validate_move(const Move &move, FigureColor color) const;

    bool is_check(FigureColor color) const;
    bool is_check(FigureColor color, const Move &move);

    void castling(FigureColor color, const Move &move);
    void en_passant(FigureColor color, const Move &move);

    void update_check_state();
    void update_move_state();

    bool try_promote_pawn(const Figure *figure, Position pos);
    void after_move_logic();

    GameState is_mate();
    bool is_stalemate(FigureColor color);
    bool is_draw() const;

    void subscribe(Subscriber *sub) override;
    void unsubscribe(Subscriber *sub) override;
    void notify() override;
};

} // namespace Chess
