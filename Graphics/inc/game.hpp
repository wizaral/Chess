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

#if defined(_WIN64) || defined(_WIN32)
#define DIR_SEP "\\"
#elif defined(__APPLE__) || defined(__linux__)
#define DIR_SEP "/"
#endif

constexpr inline int window_width = 800;
constexpr inline int window_height = 900;

class Game {
    using Sprite = std::pair<sf::Sprite, sf::Texture>;

    sf::RenderWindow &window_;
    inline const static float tile_size_ = 100.0f;
    Chess::GameState state_ = Chess::GameState::NormalMove;
    std::ostringstream log_;

    Sprite board_;
    sf::Font font_;

    std::array<Sprite, 2> promotions_;
    std::array<std::array<Sprite, 6>, 2> figures_;
    Chess::Position dragging_{-1, -1}, promoting_{-1, -1};
    sf::Vector2f dragg_pos_;

    std::optional<Chess::Move> move_;
    std::unique_ptr<Chess::Logic> logic_;
    std::unique_ptr<Chess::FigureFactory> factory_;

public:
    Game(sf::RenderWindow &window);

    void menu();
    void loop();

    static Chess::Position transform(const sf::Vector2i &pos);
    static sf::Vector2f transform(const Chess::Position &pos);

    static bool is_exit(const sf::Event &e);

private:
    void load();
    void load(Sprite &sprt, const std::string &path);

    void reprint();
    void print_board();
    void print_next_step();
    void print_state();
    void print_endgame();

    void pawn_promotion();
    void castling();
    void after_game();
    void log();

    inline const static std::array<std::string, static_cast<int>(Chess::GameState::SIZE)> states{
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
