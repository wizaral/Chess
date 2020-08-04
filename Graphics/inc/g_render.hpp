#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include "render.hpp"

#if defined(_WIN64) || defined(_WIN32)
#define DIR_SEP "\\"
#elif defined(__APPLE__) || defined(__linux__)
#define DIR_SEP "/"
#endif

constexpr inline int window_width = 800;
constexpr inline int window_height = 900;

class GraphicsRender : public Chess::Render {
    using Sprite = std::pair<sf::Sprite, sf::Texture>;

    sf::RenderWindow &window_;
    Sprite board_;
    std::array<std::array<Sprite, 6>, 2> figures_;
    std::array<Sprite, 2> promotions_;
    sf::Font font_;

    inline const static float tile_size = 100.0f;
public:
    GraphicsRender(sf::RenderWindow &window);
    void show_menu() override;
    void show_board(const Chess::Logic::Board& board) override;
    void show_next_step(Chess::Logic::Player* player) override;
    void show_error(Chess::Logic::GameState state) override;
    void show_pawn_promotion(const Chess::Logic::Board& board, const Chess::Logic::Position &pos) override;
    void show_endgame(Chess::Logic::GameState state, Chess::Logic::Player* winer) override;

    static Chess::Logic::Position transform(const sf::Vector2i& pos);
    static sf::Vector2f transform(const Chess::Logic::Position& pos);
private:
    void load(Sprite &sprt, const std::string &path);

    inline const static std::map<Chess::Logic::GameState, std::string> states{
        {Chess::Logic::GameState::CastlingFigureOnPath, "Castling: figure on path"},
        {Chess::Logic::GameState::CastlingKingInCheck, "Castling: king in check"},
        {Chess::Logic::GameState::CastlingSquareInCheck, "Castling: some square in check"},
        {Chess::Logic::GameState::CheckMate, "Checkmate"},
        {Chess::Logic::GameState::Draw, "Draw"},
        {Chess::Logic::GameState::EmptySquare, "Empty square"},
        {Chess::Logic::GameState::FailEnPassant, "Fail en passant"},
        {Chess::Logic::GameState::FiguresAlreadyMoved, "Castling: figures already moved"},
        {Chess::Logic::GameState::KingInCheck, "King in check"},
        {Chess::Logic::GameState::KingWillBeInCheck, "King will be in check"},
        {Chess::Logic::GameState::OtherFigureOnPath, "Other figure on path"},
        {Chess::Logic::GameState::OutOfBounds, "Move out of bounds"},
        {Chess::Logic::GameState::PawnStepBack, "Pawn can not move back"},
        {Chess::Logic::GameState::StaleMate, "Stalemate"},
        {Chess::Logic::GameState::WrongFigureColor, "Wrong figure color"},
        {Chess::Logic::GameState::WrongFigureMove, "Wrong figure move"},
    };
};
