#include "chess.hpp"

namespace Chess {

ChessGame::ChessGame(std::array<std::unique_ptr<Logic::Player>, 2> arr) : players_(std::move(arr)) {
    validate_players();
}

Logic::Player *ChessGame::player() {
    return players_[player_index_ %= 2].get();
}

const Logic::Board &ChessGame::board() const {
    return board_;
}

void ChessGame::init_game(std::unique_ptr<FigureFactory> factory) {
    factory->init_game(board_, player_index_, this);
}

void ChessGame::validate_players() const {
    if (players_[0]->color() == players_[1]->color()) {
        throw std::logic_error("Players have same figure colors");
    }
}

void ChessGame::subscribe(Subscriber *sub) {
    subs_.push_back(sub);
}

void ChessGame::unsubscribe(Subscriber *sub) {
    subs_.remove(sub);
}

void ChessGame::notify() {
    for (auto it = subs_.begin(); it != subs_.end();) {
        if ((*it)->update(Subscriber::MessageType::Notify) == false) {
            it = subs_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace Chess
