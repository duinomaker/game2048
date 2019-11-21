#include "Board.hpp"
#include "Direction.hpp"
#include <cstdint>
#include <iostream>
#include <list>

namespace game2048 {

using namespace std;

Board::Board(uint64_t board)
    : m_lastMove(Direction::UNKNOWN)
    , m_board(board)
{
}

Board::Board(const Board& board)
    : m_lastMove(Direction::UNKNOWN)
    , m_board(board.m_board)
{
}

Board::Board(int board[4][4])
    : m_lastMove(Direction::UNKNOWN)
    , m_board(0)
{
    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            SET_UNIT(m_board, i, j, board[i][j]);
}

void Board::reset(uint64_t board)
{
    m_lastMove = Direction::UNKNOWN;
    m_board = board;
}

void Board::reset(int board[4][4])
{
    m_lastMove = Direction::UNKNOWN;
    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            SET_UNIT(m_board, i, j, board[i][j]);
}

void Board::reset(const Board& board)
{
    m_lastMove = Direction::UNKNOWN;
    m_board = board.m_board;
}

void Board::toArray(int board[4][4]) const
{
    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            board[i][j] = static_cast<int>(GET_UNIT(m_board, i, j));
}

void Board::print() const
{
    unsigned temp;
    for (int i = 0; i != 4; ++i) {
        for (int j = 0; j != 4; ++j) {
            temp = GET_UNIT(m_board, i, j);
            if (temp)
                cout << (1 << temp) << '\t';
            else
                cout << "-\t";
        }
        cout << "\n\n";
    }
    cout << flush;
}

bool Board::isLegalMove(Direction direction) const
{
    bool found_space;
    unsigned last_number;

    switch (direction) {
    case Direction::UP:
        for (int col = 0; col != 4; ++col) {
            found_space = false;
            last_number = 0;
            for (int row = 0; row != 4; ++row) {
                if (unsigned temp = GET_UNIT(m_board, row, col)) {
                    if (found_space)
                        return true;
                    else if (temp == last_number)
                        return true;
                    last_number = temp;
                } else
                    found_space = true;
            }
        }
        break;
    case Direction::DOWN:
        for (int col = 0; col != 4; ++col) {
            found_space = false;
            last_number = 0;
            for (int row = 3; row != -1; --row) {
                if (unsigned temp = GET_UNIT(m_board, row, col)) {
                    if (found_space)
                        return true;
                    else if (temp == last_number)
                        return true;
                    last_number = temp;
                } else
                    found_space = true;
            }
        }
        break;
    case Direction::LEFT:
        for (int row = 0; row != 4; ++row) {
            found_space = false;
            last_number = 0;
            for (int col = 0; col != 4; ++col) {
                if (unsigned temp = GET_UNIT(m_board, row, col)) {
                    if (found_space)
                        return true;
                    else if (temp == last_number)
                        return true;
                    last_number = temp;
                } else
                    found_space = true;
            }
        }
        break;
    case Direction::RIGHT:
        for (int row = 0; row != 4; ++row) {
            found_space = false;
            last_number = 0;
            for (int col = 3; col != -1; --col) {
                if (unsigned temp = GET_UNIT(m_board, row, col)) {
                    if (found_space)
                        return true;
                    else if (temp == last_number)
                        return true;
                    last_number = temp;
                } else
                    found_space = true;
            }
        }
        break;
    case Direction::UNKNOWN:
    default:
        break;
    }

    return false;
}

int Board::performMove(Direction direction)
{
    uint64_t board_new = 0;
    int score_delta = 0;
    int pos_begin, pos_end, offset;
    bool found;
    for (int index = 0; index != 4; ++index) {
        if (direction == Direction::LEFT || direction == Direction::UP)
            offset = pos_begin = 0;
        else // (direction == Direction::RIGHT || direction == Direction::DOWN)
            offset = pos_begin = 3;
        if (direction == Direction::LEFT)
            while (pos_begin != 4 && !GET_UNIT(m_board, index, pos_begin))
                ++pos_begin;
        else if (direction == Direction::RIGHT)
            while (pos_begin != -1 && !GET_UNIT(m_board, index, pos_begin))
                --pos_begin;
        else if (direction == Direction::UP)
            while (pos_begin != 4 && !GET_UNIT(m_board, pos_begin, index))
                ++pos_begin;
        else // (direction == Direction::DOWN)
            while (pos_begin != -1 && !GET_UNIT(m_board, pos_begin, index))
                --pos_begin;
        if (((direction == Direction::LEFT || direction == Direction::UP) && pos_begin > 3) || ((direction == Direction::RIGHT || direction == Direction::DOWN) && pos_begin < 0))
            continue;
        for (;;) {
            found = false;
            if (direction == Direction::LEFT || direction == Direction::UP)
                pos_end = pos_begin + 1;
            else // (direction == Direction::RIGHT || direction == Direction::DOWN)
                pos_end = pos_begin - 1;
            if (((direction == Direction::LEFT || direction == Direction::UP) && pos_end != 4) || ((direction == Direction::RIGHT || direction == Direction::DOWN) && pos_end != -1)) {
                for (;;) {
                    if (direction == Direction::LEFT || direction == Direction::RIGHT) {
                        if (unsigned temp = GET_UNIT(m_board, index, pos_end)) {
                            if (temp == GET_UNIT(m_board, index, pos_begin))
                                found = true;
                            break;
                        }
                    } else // (direction == Direction::UP || direction == Direction::DOWN)
                    {
                        if (unsigned temp = GET_UNIT(m_board, pos_end, index)) {
                            if (temp == GET_UNIT(m_board, pos_begin, index))
                                found = true;
                            break;
                        }
                    }
                    if (direction == Direction::LEFT || direction == Direction::UP) {
                        if (++pos_end == 4)
                            break;
                    } else // (direction == Direction::RIGHT || direction == Direction::DOWN)
                    {
                        if (--pos_end == -1)
                            break;
                    }
                }
            }
            if (found) {
                if (direction == Direction::LEFT || direction == Direction::RIGHT) {
                    unsigned temp = GET_UNIT(m_board, index, pos_begin) + 1;
                    SET_UNIT(board_new, index, offset, temp);
                    score_delta += (1 << temp);
                } else // (direction == Direction::UP || direction == Direction::DOWN)
                {
                    unsigned temp = GET_UNIT(m_board, pos_begin, index) + 1;
                    SET_UNIT(board_new, offset, index, temp);
                    score_delta += (1 << temp);
                }
                if (direction == Direction::LEFT || direction == Direction::UP)
                    pos_begin = pos_end + 1;
                else // (direction == Direction::RIGHT || direction == Direction::DOWN)
                    pos_begin = pos_end - 1;
            } else {
                if (direction == Direction::LEFT || direction == Direction::RIGHT)
                    SET_UNIT(board_new, index, offset, GET_UNIT(m_board, index, pos_begin));
                else // (direction == Direction::UP || direction == Direction::DOWN)
                    SET_UNIT(board_new, offset, index, GET_UNIT(m_board, pos_begin, index));
                pos_begin = pos_end;
            }
            if (direction == Direction::LEFT || direction == Direction::UP) {
                ++offset;
                if (pos_begin > 3)
                    break;
            } else // (direction == Direction::RIGHT || direction == Direction::DOWN)
            {
                --offset;
                if (pos_begin < 0)
                    break;
            }
        }
    }
    m_board = board_new;
    m_lastMove = direction;
    return score_delta;
}

list<Board> Board::getComputerMovedBoards() const
{
    list<Board> boards;
    for (int row = 0; row != 4; ++row) {
        for (int col = 0; col != 4; ++col) {
            if (!GET_UNIT(m_board, row, col)) {
                for (int val : { 1, 2 }) {
                    boards.emplace_back(m_board);
                    SET_UNIT(boards.back().m_board, row, col, val);
                }
            }
        }
    }
    return boards;
}

} // namespace game2048