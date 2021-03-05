#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <sstream>
#include <string>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#endif

#include "player_base.hpp"

#if defined(_WIN64) || defined(_WIN32)

class BotPlayer : public PlayerBase {
    STARTUPINFO m_sui;
    SECURITY_ATTRIBUTES m_sattr = {sizeof(SECURITY_ATTRIBUTES), nullptr, true};
    PROCESS_INFORMATION m_pinfo;

    HANDLE m_pipe_in_w = nullptr;
    HANDLE m_pipe_in_r = nullptr;
    HANDLE m_pipe_out_w = nullptr;
    HANDLE m_pipe_out_r = nullptr;

    char m_promote_figure = '\0';
    DWORD m_write, m_read, m_available;

    inline static constexpr DWORD s_buff_size = 4096;

protected:
    std::optional<Chess::Move> &m_move;
    std::ostringstream &m_log;

public:
    BotPlayer(Chess::FigureColor color, const std::string &name, std::optional<Chess::Move> &move, std::ostringstream &log, std::string &path);
    ~BotPlayer();

    Chess::Move get_next_move() override;
    Chess::FigureType promote_figure(Chess::Position pos) override;

protected:
    static Chess::Position transform(char col, char row);
};

#elif defined(__APPLE__) || defined(__linux__)
// #include unix headers

class BotPlayer : public PlayerBase {
    // TODO bot for UNIX systems
};

#else
#error unsupported OS
#endif

class GraphicsBotPlayer : public BotPlayer {
    const float m_half_tile;
    Chess::Position &m_dragging;
    sf::Vector2f &m_dragg_pos;

    sf::RenderWindow &m_window;
    std::optional<Chess::Move> m_save;

public:
    GraphicsBotPlayer(Chess::FigureColor color, const std::string &name, std::optional<Chess::Move> &move, std::ostringstream &log, std::string &path, float tile_size, sf::RenderWindow &window, sf::Vector2f &dragg_pos, Chess::Position &dragging);
    ~GraphicsBotPlayer() = default;

    Chess::Move get_next_move() override;
};
