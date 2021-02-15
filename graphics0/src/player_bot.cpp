#include "game.hpp"

#if defined(_WIN64) || defined(_WIN32)

BotPlayer::BotPlayer(Chess::FigureColor color,
                     const std::string &name,
                     std::optional<Chess::Move> &move,
                     std::ostringstream &log,
                     std::wstring &path)
    : Chess::Player(color, name)
    , m_move(move)
    , m_log(log) {
    m_sui.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    m_sui.wShowWindow = SW_HIDE;

    CreatePipe(&m_pipe_out_r, &m_pipe_out_w, &m_sattr, s_buff_size - 1);
    SetHandleInformation(m_pipe_out_r, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&m_pipe_in_r, &m_pipe_in_w, &m_sattr, s_buff_size - 1);
    SetHandleInformation(m_pipe_in_w, HANDLE_FLAG_INHERIT, 0);

    std::memset(&m_sui, 0, sizeof(m_sui));
    std::memset(&m_pinfo, 0, sizeof(m_pinfo));

    m_sui.cb = sizeof(m_sui);
    m_sui.hStdInput = m_pipe_in_r;
    m_sui.hStdOutput = m_pipe_out_w;
    m_sui.hStdError = m_pipe_out_w;
    m_sui.dwFlags |= STARTF_USESTDHANDLES;

    CreateProcess(nullptr, path.data(), nullptr, nullptr, true, 0, nullptr, nullptr, &m_sui, &m_pinfo);
}

BotPlayer::~BotPlayer() {
    WriteFile(m_pipe_in_w, "quit\n", 5, &m_write, nullptr);

    if (m_pipe_in_w != nullptr) {
        CloseHandle(m_pipe_in_w);
    }
    if (m_pipe_in_r != nullptr) {
        CloseHandle(m_pipe_in_r);
    }
    if (m_pipe_out_w != nullptr) {
        CloseHandle(m_pipe_out_w);
    }
    if (m_pipe_out_r != nullptr) {
        CloseHandle(m_pipe_out_r);
    }
    if (m_pinfo.hProcess != nullptr) {
        CloseHandle(m_pinfo.hProcess);
    }
    if (m_pinfo.hThread != nullptr) {
        CloseHandle(m_pinfo.hThread);
    }
}

Chess::Move BotPlayer::get_next_move() {
    BYTE buffer[s_buff_size];

    std::ostringstream os, is;
    os << "position startpos moves" << m_log.str() << "\ngo\n";

    WriteFile(m_pipe_in_w, os.str().data(), os.str().length(), &m_write, nullptr);
    Sleep(100);
    PeekNamedPipe(m_pipe_out_r, buffer, s_buff_size, &m_read, &m_available, nullptr);

    do {
        Sleep(50);
        std::memset(buffer, 0, s_buff_size);

        if (!ReadFile(m_pipe_out_r, buffer, s_buff_size - 1, &m_read, nullptr) || m_read == 0)
            break;

        is << buffer;
    } while (m_read >= s_buff_size - 1);

    if (size_t n = is.str().find("bestmove"); n != std::string::npos) {
        std::string move = is.str().substr(n + 9, 5);
        m_promote_figure = move[4];
        m_move = Chess::Move{transform(move[0], move[1]), transform(move[2], move[3])};
    } else {
        throw std::runtime_error("stockfish error");
    }

    return {{-1, -1}, {-1, -1}}; // I love OOP & OOD
}

Chess::FigureType BotPlayer::promote_figure(const Chess::Position &pos) {
    switch (m_promote_figure) {
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
, m_half_tile(tile_size / 2)
, m_dragging(dragging)
, m_dragg_pos(dragg_pos)
, m_window(window) {}

Chess::Move GraphicsBotPlayer::get_next_move() {
    sf::Event event;

    while (m_window.pollEvent(event)) {
        if (Game::is_exit(event)) {
            m_window.close();
        }
    }

    if (m_save.has_value() == false) {
        BotPlayer::get_next_move();

        m_save = m_move;
        m_move.reset();
        m_dragging = m_save->from();
        m_dragg_pos = Game::transform(m_dragging);
    } else if (sf::Vector2f dest{Game::transform(m_save->to())}; m_dragg_pos == dest) {
        m_dragging = Chess::Position{-1, -1};
        m_move = m_save;
        m_save.reset();
    } else {
        auto delta = (dest - Game::transform(m_save->from())) / 25.f;
        m_dragg_pos += delta;
    }

    return {{-1, -1}, {-1, -1}}; // I love OOP & OOD
}

#elif defined(__APPLE__) || defined(__linux__)
#endif
