#include "file_factory.hpp"

FileFactory::FileFactory(const std::string &ifile_name) : m_ifile_name(ifile_name) {}

void FileFactory::change_source(const std::string &ifile_name) {
    m_ifile_name = ifile_name;
}

void FileFactory::init_game(Chess::Board &board, int &player_index, Publisher *publisher) {
    if (std::ifstream ifile(m_ifile_name); ifile) {
        set_player(ifile, player_index);
        get_last_move(ifile);
        std::string figure;

        for (int i = 0; i < Chess::board_rows; ++i) {
            for (int j = 0; j < Chess::board_cols; ++j) {
                if (ifile >> figure; figure != "---") {
                    spawn_figure(board, publisher, figure, i, j);
                }
            }
        }
    }
}

void FileFactory::set_player(std::ifstream &ifile, int &player_index) {
    std::string input;
    ifile >> input;
    player_index = input == "b";
}

void FileFactory::get_last_move(std::ifstream &ifile) {
    std::string last_move;
    ifile >> last_move;

    if (last_move != "----") {
        m_last_move = {{56 - last_move[1], last_move[0] - 97}, {56 - last_move[3], last_move[2] - 97}};

        if (Chess::Position::validation(m_last_move.from()) == false) {
            throw std::logic_error("First move start position out of bounds");
        }
        if (Chess::Position::validation(m_last_move.to()) == false) {
            throw std::logic_error("First move end position out of bounds");
        }
    }
}

void FileFactory::spawn_figure(Chess::Board &board, Publisher *publisher, const std::string &figure, int row, int col) {
    Chess::FigureColor color = figure[0] == 'w' ? Chess::FigureColor::White : Chess::FigureColor::Black;

    switch (char type = figure[1]; type) {
    case 'P':
        spawn_pawn(board, color, figure, row, col, publisher);
        break;
    case 'R':
        spawn_rook(board, color, figure, row, col);
        break;
    case 'K':
        spawn_king(board, color, figure, row, col);
        break;
    case 'B':
        board.add_figure({Chess::FigureType::Bishop, color, std::make_unique<Chess::BishopStrategy>()}, {row, col});
        break;
    case 'N':
        board.add_figure({Chess::FigureType::Knight, color, std::make_unique<Chess::KingStrategy>()}, {row, col});
        break;
    case 'Q':
        board.add_figure({Chess::FigureType::Queen, color, std::make_unique<Chess::QueenStrategy>()}, {row, col});
        break;
    default:
        break;
    }
}

void FileFactory::spawn_pawn(Chess::Board &board, Chess::FigureColor color, const std::string &figure, int row, int col, Publisher *publisher) {
    if (figure[2] != '-') {
        if (row == m_last_move.to().row() && col == m_last_move.to().col()) {
            // to save en passant state
            board.add_figure({Chess::FigureType::Pawn, color, std::make_unique<Chess::PawnStrategy>(publisher, color)}, {row, col});
            board.get_figure({row, col})->strategy()->move_update(m_last_move);
        } else {
            board.add_figure({Chess::FigureType::Pawn, color, std::make_unique<Chess::PawnStrategy>(publisher, color, Chess::PawnStrategy::MoveState::NormalMove)}, {row, col});
        }
    } else {
        // start position
        board.add_figure({Chess::FigureType::Pawn, color, std::make_unique<Chess::PawnStrategy>(publisher, color)}, {row, col});
    }
}

void FileFactory::spawn_king(Chess::Board &board, Chess::FigureColor color, const std::string &figure, int row, int col) {
    board.add_figure({Chess::FigureType::King, color, std::make_unique<Chess::KnightStrategy>()}, {row, col});

    if (figure[2] != '-') {
        board.get_figure({row, col})->strategy()->move_update(m_last_move);
    }
}

void FileFactory::spawn_rook(Chess::Board &board, Chess::FigureColor color, const std::string &figure, int row, int col) {
    board.add_figure({Chess::FigureType::Rook, color, std::make_unique<Chess::RookStrategy>()}, {row, col});

    if (figure[2] != '-') {
        board.get_figure({row, col})->strategy()->move_update(m_last_move);
    }
}
