#pragma once
#include <algorithm>
#include <array>
#include <memory>
#include <optional>

#include "board.hpp"
#include "figure.hpp"
#include "gamestate.hpp"
#include "input.hpp"
#include "player.hpp"
#include "render.hpp"

namespace Chess {

class Game : public Publisher {
    Logic::Board board_;
    int player_index_ = 0;
    std::array<std::unique_ptr<Logic::Player>, 2> players_;

    std::unique_ptr<Render> render_;
    std::unique_ptr<DataInput> input_;
public:
    Game(std::array<std::unique_ptr<Logic::Player>, 2> arr,
        std::unique_ptr<Render> render,
        std::unique_ptr<DataInput> input);

    void init_game();
    void loop();
private:
    void validate() const;
    void spawn_pawns(int row, Logic::FigureColor color, int direction);
    void spawn_figures(int row, Logic::FigureColor color);

    Logic::GameState logic(Logic::Move move);
    Logic::Player *get_current_player();

    Logic::GameState validate_move(Logic::Move move, Logic::FigureColor color) const;
    bool is_check(Logic::FigureColor color, Logic::Move move);
    bool is_check(Logic::FigureColor color) const;

    void castling(Logic::FigureColor color, Logic::Move move);
    void update_check_state();

    void try_transform_pawns();
    void transform_pawn(Logic::Position pos);

    bool is_mate();
    bool is_stalemate();
    bool is_draw() const;

    // Publisher part
    void subscribe(Subscriber *sub) override;
    void unsubscribe(Subscriber *sub) override;
    void notify() override;
};

} // namespace Chess::Logic
