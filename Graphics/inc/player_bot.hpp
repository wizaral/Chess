#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <sstream>
#include <string>
#include <windows.h>
#include "chess.hpp"

class BotPlayer : public Chess::Player {
    STARTUPINFO sui_;
    SECURITY_ATTRIBUTES sattr_ = {sizeof(SECURITY_ATTRIBUTES), nullptr, true};
    PROCESS_INFORMATION pinfo_;

    HANDLE pipe_in_w_ = nullptr;
    HANDLE pipe_in_r_ = nullptr;
    HANDLE pipe_out_w_ = nullptr;
    HANDLE pipe_out_r_ = nullptr;

    char promote_figure_ = '\0';
    DWORD write_, read_, available_;

    static const DWORD buff_size_ = 4096;

protected:
    std::optional<Chess::Move> &move_;
    std::ostringstream &log_;
public:
    BotPlayer(Chess::FigureColor color, const std::string &name, std::optional<Chess::Move> &move, std::ostringstream &log, std::wstring &path);
    ~BotPlayer();

    Chess::Move get_next_move() override;
    Chess::FigureType promote_figure(const Chess::Position &pos) override;

protected:
    static Chess::Position transform(char col, char row);
};

class GraphicsBotPlayer : public BotPlayer {
    const float half_tile_;
    Chess::Position &dragging_;
    sf::Vector2f &dragg_pos_;

    sf::RenderWindow &window_;
    std::optional<Chess::Move> save_;

public:
    GraphicsBotPlayer(Chess::FigureColor color, const std::string &name, std::optional<Chess::Move> &move, std::ostringstream &log, std::wstring &path, float tile_size, sf::RenderWindow &window, sf::Vector2f &dragg_pos, Chess::Position &dragging);
    ~GraphicsBotPlayer() = default;

    Chess::Move get_next_move() override;
};
