#pragma once
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <optional>

#include "factory.hpp"
#include "gamestate.hpp"
#include "observer.hpp"

namespace Chess {

class ChessGame : public Publisher {
    Logic::Board board_;
    std::array<std::unique_ptr<Logic::Player>, 2> players_;

    int player_index_ = 0;
    Logic::Position pawn_pos_{-1, -1};
    Logic::GameState state_ = Logic::GameState::NormalMove;

public:
    ChessGame(std::array<std::unique_ptr<Logic::Player>, 2> arr);

    Logic::Player *player();
    const Logic::Board &board() const;

    void init_game(std::unique_ptr<FigureFactory> factory);
    Logic::GameState logic(const Logic::Move &move);
    Logic::GameState promote_pawn(Logic::FigureType type);

private:
    void validate_players() const;
    Logic::GameState validate_move(const Logic::Move &move, Logic::FigureColor color) const;

    bool is_check(Logic::FigureColor color) const;
    bool is_check(Logic::FigureColor color, const Logic::Move &move);

    void castling(Logic::FigureColor color, const Logic::Move &move);
    void en_passant(Logic::FigureColor color, const Logic::Move &move);

    void update_check_state();
    void update_move_state();

    bool try_promote_pawn(const Logic::Figure &figure, const Logic::Position &pos);
    void after_move_logic();

    Logic::GameState is_mate();
    bool is_stalemate(Logic::FigureColor color);
    bool is_draw() const;

    // Publisher part
    void subscribe(Subscriber *sub) override;
    void unsubscribe(Subscriber *sub) override;
    void notify() override;
};

} // namespace Chess
