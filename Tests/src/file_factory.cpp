#include "file_factory.hpp"

FileFactory::FileFactory(const std::string &ifile_name) : ifile_name_(ifile_name) {}

void FileFactory::change_source(const std::string &ifile_name) {
    ifile_name_ = ifile_name;
}

void FileFactory::init_game(Chess::Logic::Board &board, int &player_index, Publisher *publisher) {
    if (std::ifstream ifile(ifile_name_); ifile) {
        set_player(ifile, player_index);
        get_last_move(ifile);
        std::string figure;

        for (int i = 0; i < Chess::Logic::board_rows; ++i) {
            for (int j = 0; j < Chess::Logic::board_cols; ++j) {
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
        last_move_ = {{56 - last_move[1], last_move[0] - 97}, {56 - last_move[3], last_move[2] - 97}};

        if (Chess::Logic::Position::validation(last_move_.from()) == false) {
            throw std::logic_error("First move start position out of bounds");
        }
        if (Chess::Logic::Position::validation(last_move_.to()) == false) {
            throw std::logic_error("First move end position out of bounds");
        }
    }
}

void FileFactory::spawn_figure(Chess::Logic::Board &board, Publisher *publisher, const std::string &figure, int row, int col) {
    Chess::Logic::FigureColor color = figure[0] == 'w' ? Chess::Logic::FigureColor::White : Chess::Logic::FigureColor::Black;

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
        board.add_figure({Chess::Logic::FigureType::Bishop, color, std::make_unique<Chess::Logic::BishopStrategy>()}, {row, col});
        break;
    case 'N':
        board.add_figure({Chess::Logic::FigureType::Knight, color, std::make_unique<Chess::Logic::KingStrategy>()}, {row, col});
        break;
    case 'Q':
        board.add_figure({Chess::Logic::FigureType::Queen, color, std::make_unique<Chess::Logic::QueenStrategy>()}, {row, col});
        break;
    default:
        break;
    }
}

void FileFactory::spawn_pawn(Chess::Logic::Board &board, Chess::Logic::FigureColor color, const std::string &figure, int row, int col, Publisher *publisher) {
    if (figure[2] != '-') {
        if (row == last_move_.to().row() && col == last_move_.to().col()) {
            // to save en passant state
            board.add_figure({Chess::Logic::FigureType::Pawn, color, std::make_unique<Chess::Logic::PawnStrategy>(publisher, color)}, {row, col});
            board.get_figure({row, col})->strategy()->move_update(last_move_);
        } else {
            board.add_figure({Chess::Logic::FigureType::Pawn, color, std::make_unique<Chess::Logic::PawnStrategy>(publisher, color, Chess::Logic::PawnStrategy::MoveState::NormalMove)}, {row, col});
        }
    } else {
        // start position
        board.add_figure({Chess::Logic::FigureType::Pawn, color, std::make_unique<Chess::Logic::PawnStrategy>(publisher, color)}, {row, col});
    }
}

void FileFactory::spawn_king(Chess::Logic::Board &board, Chess::Logic::FigureColor color, const std::string &figure, int row, int col) {
    board.add_figure({Chess::Logic::FigureType::King, color, std::make_unique<Chess::Logic::KnightStrategy>()}, {row, col});

    if (figure[2] != '-') {
        board.get_figure({row, col})->strategy()->move_update(last_move_);
    }
}

void FileFactory::spawn_rook(Chess::Logic::Board &board, Chess::Logic::FigureColor color, const std::string &figure, int row, int col) {
    board.add_figure({Chess::Logic::FigureType::Rook, color, std::make_unique<Chess::Logic::RookStrategy>()}, {row, col});

    if (figure[2] != '-') {
        board.get_figure({row, col})->strategy()->move_update(last_move_);
    }
}
