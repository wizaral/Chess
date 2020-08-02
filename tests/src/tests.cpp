#include <iostream>
#include "t_input.hpp"
#include "t_render.hpp"
#include "tests.hpp"
#include "test_runner.hpp"

void test0() {
    TestGame game = init("tests\\1.txt");
    game.init_game();
    game.render()->show_board(game.board());

    std::cout << "D1" << std::endl;
    std::array<std::array<std::unique_ptr<Chess::Logic::Figure>, 8>, 8> correct;
    correct[4][4] = std::make_unique<Chess::Logic::Figure>(Chess::Logic::FigureType::Pawn, Chess::Logic::FigureColor::White, std::make_unique<Chess::Logic::PawnStrategy>(nullptr, Chess::Logic::FigureColor::White, Chess::Logic::PawnStrategy::MoveState::NormalMove));
    std::cout << "D1" << std::endl;

    Chess::Logic::Move move{TestInput::transform(2, 'e'), TestInput::transform(4, 'e')};
    Chess::Logic::GameState gstate = game.logic(move);

    ASSERT_EQUAL(gstate, Chess::Logic::GameState::NormalMove);
    ASSERT_EQUAL(game.figures(), correct);
}

int main() {
    // TestGame game({
    //     std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::White, "white"),
    //     std::make_unique<Chess::Logic::Player>(Chess::Logic::FigureColor::Black, "black"),
    //     }, std::make_unique<FileFactory>(), std::make_unique<ConsoleRender>(), std::make_unique<ConsoleInput>());

    // game.init_game();
    TestRunner tr(false);

    RUN_TEST(tr, test0);
    // RUN_TEST(tr, test1);
}
