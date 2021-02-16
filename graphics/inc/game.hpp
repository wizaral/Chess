#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include "chess.hpp"
#include "player_bot.hpp"
#include "player_real.hpp"

#if defined(GAME_RESOURCES_DIR)
#define RES GAME_RESOURCES_DIR
#else
#define RES "res/"
#endif

inline static constexpr int window_width = 800;
inline static constexpr int window_height = 900;

class Game {
    using Sprite = std::pair<sf::Sprite, sf::Texture>;

    sf::RenderWindow &m_window;
    inline constexpr static float s_tile_size = 100.0f;
    Chess::GameState m_state = Chess::GameState::NormalMove;
    std::ostringstream m_log;

    sf::Font m_font;
    Sprite m_board, m_background, m_rect;

    std::array<Sprite, 5> m_buttons;
    std::array<Sprite, 2> m_promotions;
    std::array<std::array<Sprite, 6>, 2> m_figures;
    Chess::Position m_dragging{-1, -1}, m_promoting{-1, -1};
    sf::Vector2f m_dragg_pos;

    std::optional<Chess::Move> m_move;
    std::unique_ptr<Chess::Logic> m_logic;
    std::unique_ptr<Chess::FigureFactory> m_factory;

public:
    Game(sf::RenderWindow &window);

    void menu();

    static Chess::Position transform(const sf::Vector2i &pos);
    static sf::Vector2f transform(const Chess::Position &pos);

    static bool is_exit(const sf::Event &e);

private:
    void load();
    void load(Sprite &sprt, const std::string &path);

    void loop();

    void reprint();
    void print_menu(std::array<int, 4> &positions);
    void print_board();
    void print_castling();
    void print_next_step();
    void print_state();
    void print_endgame();

    void pawn_promotion();
    void castling();
    void after_game();
    void log();

    inline const static std::array<std::string, static_cast<int>(Chess::GameState::SIZE)> s_states{
        "Good move",
        "En passant",
        "King castling",
        "Queen castling",
        "Pawn promotion",
        "Checkmate",
        "Stalemate",
        "Draw",
        "Move out of bounds",
        "Empty square",
        "Wrong figure color",
        "Wrong figure move",
        "Other figure on path",
        "King in check",
        "King will be in check",
        "Castling: figure already moved",
        "Castling: king in check",
        "Castling: some square in check",
        "Castling: figure on path",
        "Fail en passant",
        "Pawn can not move back",
    };
};
