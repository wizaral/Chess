#pragma once
#include "t_render.hpp"
#include "t_input.hpp"
#include "file_factory.hpp"

#if defined(_WIN64) || defined(_WIN32)
#define DIR_SEP "\\"
#elif defined(__APPLE__) || defined(__linux__)
#define DIR_SEP "/"
#endif

class TestGame : public Chess::Logic {
public:
    template<class T>
    using Field = std::array<std::array<T, 8>, 8>;

    TestGame(std::array<std::unique_ptr<Chess::Player>, 2> arr,
        std::unique_ptr<Chess::FigureFactory> factory,
        std::unique_ptr<Chess::Render> render,
        std::unique_ptr<Chess::DataInput> input)
        : Chess::Logic(std::move(arr)
        , std::move(factory)
        , std::move(render)
        , std::move(input)) {}

    Chess::GameState logic(const Chess::Move &move) {
        return Chess::Logic::logic(move);
    }

    const Field<std::unique_ptr<Chess::Figure>> &figures() const {
        return board_.figures();
    }

    const Chess::Board &board() const {
        return board_;
    }

    Chess::Render *render() {
        return render_.get();
    }
};

inline TestGame init(const std::string &file_name) {
    return TestGame({
        std::make_unique<Chess::Player>(Chess::FigureColor::White, "white"),
        std::make_unique<Chess::Player>(Chess::FigureColor::Black, "black")},
        std::make_unique<FileFactory>(file_name),
        std::make_unique<TestRender>(),
        std::make_unique<TestInput>());
}

inline std::ostream &operator<<(std::ostream &os, const std::unique_ptr<Chess::Figure> &figure) {
    return os << TestRender::colors.at(figure->color()) << TestRender::figures.at(figure->type()) << ' ';
}
