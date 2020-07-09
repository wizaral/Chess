#pragma once
#include <array>
#include <memory>

#include "board.hpp"
#include "gamestate.hpp"
#include "input.hpp"
#include "player.hpp"
#include "render.hpp"

namespace Chess::Logic {

class Game : public Publisher {
    Board board_;
    int player_index_ = 0;
    std::array<std::unique_ptr<Player>, 2> players_;

    std::unique_ptr<Render> render_;
    std::unique_ptr<DataInput> input_;
public:
    Game(std::array<std::unique_ptr<Player>, 2> arr,
        std::unique_ptr<Render> render,
        std::unique_ptr<DataInput> input);

    void init_game();
    void loop();
private:
    void validate() const;
    void spawn_pawns(int row, FigureColor color, int direction);
    void spawn_figures(int row, FigureColor color);

    GameState logic(Move move);
    void try_transform_pawns();
    void transform_pawn(Position pos);

    // Publisher part
    void subscribe(Subscriber *sub) override;
    void unsubscribe(Subscriber *sub) override;
    void notify() override;

};

} // namespace Chess::Logic
