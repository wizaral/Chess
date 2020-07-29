#pragma once
#include <algorithm>
#include <array>
#include <memory>
#include <optional>

#include "factory.hpp"
#include "gamestate.hpp"
#include "input.hpp"
#include "observer.hpp"
#include "render.hpp"

namespace Chess {

class ChessGame : public Publisher {
    Logic::Board board_;
    int player_index_ = 0;
    std::array<std::unique_ptr<Logic::Player>, 2> players_;

    std::unique_ptr<FigureFactory> factory_;
    std::unique_ptr<Render> render_;
    std::unique_ptr<DataInput> input_;

public:
    ChessGame(std::array<std::unique_ptr<Logic::Player>, 2> arr,
        std::unique_ptr<FigureFactory> factory,
        std::unique_ptr<Render> render,
        std::unique_ptr<DataInput> input);

    void init_game();
    void loop();

private:
    void validate() const;

    Logic::GameState logic(const Logic::Move &move);
    Logic::Player *get_current_player();

    Logic::GameState validate_move(const Logic::Move &move, Logic::FigureColor color) const;
    bool is_check(Logic::FigureColor color, const Logic::Move &move);
    bool is_check(Logic::FigureColor color) const;

    void castling(Logic::FigureColor color, const Logic::Move &move);
    void en_passant(Logic::FigureColor color, const Logic::Move &move);

    void update_check_state();
    void update_move_state();

    void try_transform_pawns();
    void transform_pawn(Logic::Position pos);

    Logic::GameState is_mate();
    bool is_stalemate(Logic::FigureColor color);
    bool is_draw() const;

    // Publisher part
    void subscribe(Subscriber *sub) override;
    void unsubscribe(Subscriber *sub) override;
    void notify() override;
};

} // namespace Chess
