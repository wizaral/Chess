#pragma once
#include <array>
#include <memory>

#include "board.hpp"
#include "player.hpp"

namespace Chess::Logic {

class Game : public Publisher {
    Board board_;
    int player_index = 0;
    std::array<std::unique_ptr<Player>, 2> players_;
public:
    Game(std::array<std::unique_ptr<Player>, 2> arr);

    void init_game();
    void loop();
private:
    void validate() const;
    void spawn_pawns(int row, FigureColor color, int direction);
    void spawn_figures(int row, FigureColor color);

    void try_transform_pawn();
    void transform_pawn(Position pos);

    // Publisher part
    void subscribe(Subscriber *sub) override;
    void unsubscribe(Subscriber *sub) override;
    void notify() override;
};

} // namespace Chess::Logic
