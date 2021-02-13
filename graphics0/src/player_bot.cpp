#include "game.hpp"

#if defined(_WIN64) || defined(_WIN32)

BotPlayer::BotPlayer(Chess::FigureColor color,
                     const std::string &name,
                     std::optional<Chess::Move> &move,
                     std::ostringstream &log,
                     std::wstring &path)
    : Chess::Player(color, name)
    , move_(move)
    , log_(log) {
    sui_.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    sui_.wShowWindow = SW_HIDE;

    CreatePipe(&pipe_out_r_, &pipe_out_w_, &sattr_, buff_size_ - 1);
    SetHandleInformation(pipe_out_r_, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&pipe_in_r_, &pipe_in_w_, &sattr_, buff_size_ - 1);
    SetHandleInformation(pipe_in_w_, HANDLE_FLAG_INHERIT, 0);

    std::memset(&sui_, 0, sizeof(sui_));
    std::memset(&pinfo_, 0, sizeof(pinfo_));

    sui_.cb = sizeof(sui_);
    sui_.hStdInput = pipe_in_r_;
    sui_.hStdOutput = pipe_out_w_;
    sui_.hStdError = pipe_out_w_;
    sui_.dwFlags |= STARTF_USESTDHANDLES;

    CreateProcess(nullptr, path.data(), nullptr, nullptr, true, 0, nullptr, nullptr, &sui_, &pinfo_);
}

BotPlayer::~BotPlayer() {
    WriteFile(pipe_in_w_, "quit\n", 5, &write_, nullptr);

    if (pipe_in_w_ != nullptr) {
        CloseHandle(pipe_in_w_);
    }
    if (pipe_in_r_ != nullptr) {
        CloseHandle(pipe_in_r_);
    }
    if (pipe_out_w_ != nullptr) {
        CloseHandle(pipe_out_w_);
    }
    if (pipe_out_r_ != nullptr) {
        CloseHandle(pipe_out_r_);
    }
    if (pinfo_.hProcess != nullptr) {
        CloseHandle(pinfo_.hProcess);
    }
    if (pinfo_.hThread != nullptr) {
        CloseHandle(pinfo_.hThread);
    }
}

Chess::Move BotPlayer::get_next_move() {
    BYTE buffer[buff_size_];

    std::ostringstream os, is;
    os << "position startpos moves" << log_.str() << "\ngo\n";

    WriteFile(pipe_in_w_, os.str().data(), os.str().length(), &write_, nullptr);
    Sleep(100);
    PeekNamedPipe(pipe_out_r_, buffer, buff_size_, &read_, &available_, nullptr);

    do {
        Sleep(50);
        std::memset(buffer, 0, buff_size_);

        if (!ReadFile(pipe_out_r_, buffer, buff_size_ - 1, &read_, nullptr) || read_ == 0)
            break;

        is << buffer;
    } while (read_ >= buff_size_ - 1);

    if (size_t n = is.str().find("bestmove"); n != std::string::npos) {
        std::string move = is.str().substr(n + 9, 5);
        promote_figure_ = move[4];
        move_ = Chess::Move{transform(move[0], move[1]), transform(move[2], move[3])};
    } else {
        throw std::runtime_error("stockfish error");
    }

    return {{-1, -1}, {-1, -1}}; // I love OOP & OOD
}

Chess::FigureType BotPlayer::promote_figure(const Chess::Position &pos) {
    switch (promote_figure_) {
    case 'B':
    case 'b':
        return Chess::FigureType::Bishop;
    case 'N':
    case 'n':
        return Chess::FigureType::Knight;
    case 'R':
    case 'r':
        return Chess::FigureType::Rook;
    default:
        return Chess::FigureType::Queen;
    }
}

Chess::Position BotPlayer::transform(char col, char row) {
    return {row - 49, col - 97};
}

GraphicsBotPlayer::GraphicsBotPlayer(Chess::FigureColor color,
                                     const std::string &name,
                                     std::optional<Chess::Move> &move,
                                     std::ostringstream &log,
                                     std::wstring &path,
                                     float tile_size,
                                     sf::RenderWindow &window,
                                     sf::Vector2f &dragg_pos,
                                     Chess::Position &dragging)
    : BotPlayer(color, name, move, log, path)
    , half_tile_(tile_size / 2)
    , dragging_(dragging)
    , dragg_pos_(dragg_pos)
    , window_(window) {}

Chess::Move GraphicsBotPlayer::get_next_move() {
    sf::Event event;

    while (window_.pollEvent(event)) {
        if (Game::is_exit(event)) {
            window_.close();
        }
    }

    if (save_.has_value() == false) {
        BotPlayer::get_next_move();

        save_ = move_;
        move_.reset();
        dragging_ = save_->from();
        dragg_pos_ = Game::transform(dragging_);
    } else if (sf::Vector2f dest{Game::transform(save_->to())}; dragg_pos_ == dest) {
        dragging_ = Chess::Position{-1, -1};
        move_ = save_;
        save_.reset();
    } else {
        auto delta = (dest - Game::transform(save_->from())) / 25.f;
        dragg_pos_ += delta;
    }

    return {{-1, -1}, {-1, -1}}; // I love OOP & OOD
}

#elif defined(__APPLE__)
#elif defined(__linux__)
#endif
