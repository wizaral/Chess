#pragma once
#include <fstream>
#include <string>

#include "chess.hpp"

class FileFactory final : public Chess::FigureFactory {
    std::string ifile_name_;
    Chess::Logic::Move last_move_ {{-1, -1}, {-1, -1}};
public:
    FileFactory(const std::string &ifile_name);

    void change_source(const std::string &ifile_name);
    void init_game(Chess::Logic::Board &board, int &player_index, Publisher *publisher) override;
private:
    void set_player(std::ifstream &ifile, int &player_index);
    void get_last_move(std::ifstream &ifile);

    void spawn_figure(Chess::Logic::Board &board, Publisher *publisher, const std::string &f, int row, int col);
    void last_move_figure(Chess::Logic::Board &board, Publisher *publisher, const std::string &f, int row, int col);

    // void spawn_pawn();
    // void spawn_king();
    // void spawn_rook();
};
