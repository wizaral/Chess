#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <sstream>
#include <string>
#include <windows.h>
#include "chess.hpp"

class BotPlayer : public Chess::Player {
    const float half_tile_;
    Chess::Position& dragging_;
    sf::Vector2f &dragg_pos_;
    std::ostringstream& log_;

    sf::RenderWindow& window_;
    std::optional<Chess::Move> save_, &move_;

    STARTUPINFO sui_;
    SECURITY_ATTRIBUTES sattr_ = {sizeof(SECURITY_ATTRIBUTES), nullptr, true};
    PROCESS_INFORMATION pinfo_;

    HANDLE pipe_in_w_ = nullptr;
    HANDLE pipe_in_r_ = nullptr;
    HANDLE pipe_out_w_ = nullptr;
    HANDLE pipe_out_r_ = nullptr;

    char promote_figure_ = '\0';
    DWORD write_, read_, available_;
    BYTE buffer_[4096];
public:
    BotPlayer(Chess::FigureColor color, const std::string &name, float tile_size, Chess::Position& dragging, sf::Vector2f &dragg_pos, std::ostringstream &log, sf::RenderWindow &window, std::optional<Chess::Move> &move, std::wstring& path);
    ~BotPlayer();

    Chess::Move get_next_move() override;
    Chess::FigureType promote_figure(const Chess::Position& pos) override;
private:
    static Chess::Position transform(char col, char row);
};
