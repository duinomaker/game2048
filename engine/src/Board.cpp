#include "Board.hpp"
#include "Move.hpp"
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <list>
#include <utility>

namespace game2048 {

namespace Board {

    using namespace std;

    uint64_t fromArray(int arr[][4])
    {
        uint64_t board;
        for (int i = 0; i != 4; ++i)
            for (int j = 0; j != 4; ++j)
                SET_UNIT(board, i, j, arr[i][j]);
        return board;
    }

    void toArray(uint64_t board, int arr[][4])
    {
        for (int i = 0; i != 4; ++i)
            for (int j = 0; j != 4; ++j)
                arr[i][j] = static_cast<int>(GET_UNIT(board, i, j));
    }

    bool isLegalMove(uint64_t board, Move direction)
    {
        bool found_space;
        unsigned last_number;

        switch (direction) {
        case Move::UP:
            for (int col = 0; col != 4; ++col) {
                found_space = false;
                last_number = 0;
                for (int row = 0; row != 4; ++row) {
                    if (unsigned temp = GET_UNIT(board, row, col)) {
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
        case Move::DOWN:
            for (int col = 0; col != 4; ++col) {
                found_space = false;
                last_number = 0;
                for (int row = 3; row != -1; --row) {
                    if (unsigned temp = GET_UNIT(board, row, col)) {
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
        case Move::LEFT:
            for (int row = 0; row != 4; ++row) {
                found_space = false;
                last_number = 0;
                for (int col = 0; col != 4; ++col) {
                    if (unsigned temp = GET_UNIT(board, row, col)) {
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
        case Move::RIGHT:
            for (int row = 0; row != 4; ++row) {
                found_space = false;
                last_number = 0;
                for (int col = 3; col != -1; --col) {
                    if (unsigned temp = GET_UNIT(board, row, col)) {
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

        case Move::COMPUTER_1:
        case Move::COMPUTER_2:
        case Move::UNKNOWN:
        default:
            break;
        }

        return false;
    }

    int performMove(uint64_t& board, Move direction)
    {
        uint64_t board_new = 0;
        int score_delta = 0;
        int pos_begin, pos_end, offset;
        bool found;
        for (int index = 0; index != 4; ++index) {
            if (direction == Move::LEFT || direction == Move::UP)
                offset = pos_begin = 0;
            else // (direction == Direction::RIGHT || direction == Direction::DOWN)
                offset = pos_begin = 3;
            if (direction == Move::LEFT)
                while (pos_begin != 4 && !GET_UNIT(board, index, pos_begin))
                    ++pos_begin;
            else if (direction == Move::RIGHT)
                while (pos_begin != -1 && !GET_UNIT(board, index, pos_begin))
                    --pos_begin;
            else if (direction == Move::UP)
                while (pos_begin != 4 && !GET_UNIT(board, pos_begin, index))
                    ++pos_begin;
            else // (direction == Direction::DOWN)
                while (pos_begin != -1 && !GET_UNIT(board, pos_begin, index))
                    --pos_begin;
            if (((direction == Move::LEFT || direction == Move::UP) && pos_begin > 3) || ((direction == Move::RIGHT || direction == Move::DOWN) && pos_begin < 0))
                continue;
            for (;;) {
                found = false;
                if (direction == Move::LEFT || direction == Move::UP)
                    pos_end = pos_begin + 1;
                else // (direction == Direction::RIGHT || direction == Direction::DOWN)
                    pos_end = pos_begin - 1;
                if (((direction == Move::LEFT || direction == Move::UP) && pos_end != 4) || ((direction == Move::RIGHT || direction == Move::DOWN) && pos_end != -1)) {
                    for (;;) {
                        if (direction == Move::LEFT || direction == Move::RIGHT) {
                            if (unsigned temp = GET_UNIT(board, index, pos_end)) {
                                if (temp == GET_UNIT(board, index, pos_begin))
                                    found = true;
                                break;
                            }
                        } else // (direction == Direction::UP || direction == Direction::DOWN)
                        {
                            if (unsigned temp = GET_UNIT(board, pos_end, index)) {
                                if (temp == GET_UNIT(board, pos_begin, index))
                                    found = true;
                                break;
                            }
                        }
                        if (direction == Move::LEFT || direction == Move::UP) {
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
                    if (direction == Move::LEFT || direction == Move::RIGHT) {
                        unsigned temp = GET_UNIT(board, index, pos_begin) + 1;
                        SET_UNIT(board_new, index, offset, temp);
                        score_delta += (1 << temp);
                    } else // (direction == Direction::UP || direction == Direction::DOWN)
                    {
                        unsigned temp = GET_UNIT(board, pos_begin, index) + 1;
                        SET_UNIT(board_new, offset, index, temp);
                        score_delta += (1 << temp);
                    }
                    if (direction == Move::LEFT || direction == Move::UP)
                        pos_begin = pos_end + 1;
                    else // (direction == Direction::RIGHT || direction == Direction::DOWN)
                        pos_begin = pos_end - 1;
                } else {
                    if (direction == Move::LEFT || direction == Move::RIGHT)
                        SET_UNIT(board_new, index, offset, GET_UNIT(board, index, pos_begin));
                    else // (direction == Direction::UP || direction == Direction::DOWN)
                        SET_UNIT(board_new, offset, index, GET_UNIT(board, pos_begin, index));
                    pos_begin = pos_end;
                }
                if (direction == Move::LEFT || direction == Move::UP) {
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
        board = board_new;
        return score_delta;
    }

    void performMove(uint64_t& board, Move val, const pair<int, int>& coord)
    {
        SET_UNIT(board, coord.first, coord.second, static_cast<int>(val));
    }

    list<pair<int, int>> getAllSlots(uint64_t board)
    {
        list<pair<int, int>> slots;
        for (int row = 0; row != 4; ++row)
            for (int col = 0; col != 4; ++col)
                if (!GET_UNIT(board, row, col))
                    slots.emplace_back(row, col);
        return slots;
    }

    void represent(uint64_t board, std::ostream& out)
    {
        unsigned val;
        out.flags(ios::left);
        for (int i = 0; i != 4; ++i) {
            for (int j = 0; j != 4; ++j) {
                val = GET_UNIT(board, i, j);
                if (val)
                    out << setw(6) << (1 << val);
                else
                    out << setw(6) << ".";
            }
            if (i != 3)
                out << "\n\n";
            else
                out << flush;
        }
    }

} // namespace Board

} // namespace game2048