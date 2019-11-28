#include "Board.hpp"
#include "Move.hpp"
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

namespace game2048 {

namespace Board {

    using namespace std;

    array<board_t, 65536> effect_up;
    array<board_t, 65536> effect_down;
    array<board_t, 65536> effect_left;
    array<board_t, 65536> effect_right;

    static void _performRowMove(row_t& row, Move direction)
    {
        row_t row_new = 0;
        int score_delta = 0;
        int pos_begin, pos_end, offset;
        bool found;
        if (direction == Move::LEFT)
            offset = pos_begin = 0;
        else // (direction == Direction::RIGHT)
            offset = pos_begin = 3;
        if (direction == Move::LEFT)
            while (pos_begin != 4 && !GET_ROW_UNIT(row, pos_begin))
                ++pos_begin;
        else // (direction == Move::RIGHT)
            while (pos_begin != -1 && !GET_ROW_UNIT(row, pos_begin))
                --pos_begin;
        if ((direction == Move::LEFT && pos_begin > 3) || (direction == Move::RIGHT && pos_begin < 0))
            return;
        for (;;) {
            found = false;
            if (direction == Move::LEFT)
                pos_end = pos_begin + 1;
            else // (direction == Direction::RIGHT)
                pos_end = pos_begin - 1;
            if ((direction == Move::LEFT && pos_end != 4) || (direction == Move::RIGHT && pos_end != -1)) {
                for (;;) {
                    if (int temp = GET_ROW_UNIT(row, pos_end)) {
                        if (temp == GET_ROW_UNIT(row, pos_begin))
                            found = true;
                        break;
                    }
                    if (direction == Move::LEFT) {
                        if (++pos_end == 4)
                            break;
                    } else // (direction == Direction::RIGHT)
                    {
                        if (--pos_end == -1)
                            break;
                    }
                }
            }
            if (found) {
                int temp = GET_ROW_UNIT(row, pos_begin) + 1;
                SET_ROW_UNIT(row_new, offset, temp);
                score_delta += (1 << temp);
                if (direction == Move::LEFT)
                    pos_begin = pos_end + 1;
                else // (direction == Direction::RIGHT)
                    pos_begin = pos_end - 1;
            } else {
                SET_ROW_UNIT(row_new, offset, GET_ROW_UNIT(row, pos_begin));
                pos_begin = pos_end;
            }
            if (direction == Move::LEFT) {
                ++offset;
                if (pos_begin > 3)
                    break;
            } else // (direction == Direction::RIGHT)
            {
                --offset;
                if (pos_begin < 0)
                    break;
            }
        }
        row = row_new;
    }

    static board_t _toColumn(board_t row)
    {
        return (((row >> 0) & UINT64_C(0x000000000000000F)) << 0)
            | (((row >> 4) & UINT64_C(0x000000000000000F)) << 16)
            | (((row >> 8) & UINT64_C(0x000000000000000F)) << 32)
            | (((row >> 12) & UINT64_C(0x000000000000000F)) << 48);
    }

    void initialize()
    {
        row_t row_new;
        for (row_t row = 1; row != 0; ++row) {
            row_new = row;
            _performRowMove(row_new, Move::LEFT);
            effect_left[row] = row_new ^ row;
            effect_up[row] = _toColumn(row_new) ^ _toColumn(row);
            row_new = row;
            _performRowMove(row_new, Move::RIGHT);
            effect_right[row] = row_new ^ row;
            effect_down[row] = _toColumn(row_new) ^ _toColumn(row);
        }
    }

    board_t fromArray(int arr[][4])
    {
        board_t board;
        for (int i = 0; i != 4; ++i)
            for (int j = 0; j != 4; ++j)
                SET_BOARD_UNIT(board, i, j, arr[i][j]);
        return board;
    }

    void toArray(board_t board, int arr[][4])
    {
        for (int i = 0; i != 4; ++i)
            for (int j = 0; j != 4; ++j)
                arr[i][j] = static_cast<int>(GET_BOARD_UNIT(board, i, j));
    }

    board_t transpose(board_t board)
    {
        board_t a1 = board & UINT64_C(0xF0F00F0FF0F00F0F);
        board_t a2 = board & UINT64_C(0x0000F0F00000F0F0);
        board_t a3 = board & UINT64_C(0x0F0F00000F0F0000);
        board_t a = a1 | (a2 << 12) | (a3 >> 12);
        board_t b1 = a & UINT64_C(0xFF00FF0000FF00FF);
        board_t b2 = a & UINT64_C(0x00FF00FF00000000);
        board_t b3 = a & UINT64_C(0x00000000FF00FF00);
        return b1 | (b2 >> 24) | (b3 << 24);
    }

    board_t transpose_prime(board_t board)
    {
        board_t a1 = board & UINT64_C(0x0F0FF0F00F0FF0F0);
        board_t a2 = board & UINT64_C(0x00000F0F00000F0F);
        board_t a3 = board & UINT64_C(0xF0F00000F0F00000);
        board_t a = a1 | (a2 << 20) | (a3 >> 20);
        board_t b1 = a & UINT64_C(0x00FF00FFFF00FF00);
        board_t b2 = a & UINT64_C(0xFF00FF0000000000);
        board_t b3 = a & UINT64_C(0x0000000000FF00FF);
        return b1 | (b2 >> 40) | (b3 << 40);
    }

    board_t flip_row(board_t board)
    {
        board_t r1 = board & UINT64_C(0xFFFF000000000000);
        board_t r2 = board & UINT64_C(0x0000FFFF00000000);
        board_t r3 = board & UINT64_C(0x00000000FFFF0000);
        board_t r4 = board & ROW_MASK;
        return (r1 >> 48) | (r2 >> 16) | (r3 << 16) | (r4 << 48);
    }

    board_t flip_column(board_t board)
    {
        board_t c1 = board & UINT64_C(0xF000F000F000F000);
        board_t c2 = board & UINT64_C(0x0F000F000F000F00);
        board_t c3 = board & UINT64_C(0x00F000F000F000F0);
        board_t c4 = board & UINT64_C(0x000F000F000F000F);
        return (c1 >> 12) | (c2 >> 4) | (c3 << 4) | (c4 << 12);
    }

    board_t rotate_left(board_t board)
    {
        board_t a1 = board & UINT64_C(0xF0F00000F0F00000);
        board_t a2 = board & UINT64_C(0x0F0F00000F0F0000);
        board_t a3 = board & UINT64_C(0x0000F0F00000F0F0);
        board_t a4 = board & UINT64_C(0x00000F0F00000F0F);
        board_t a = (a1 >> 16) | (a2 << 4) | (a3 >> 4) | (a4 << 16);
        board_t b1 = a & UINT64_C(0xFF00FF0000000000);
        board_t b2 = a & UINT64_C(0x00FF00FF00000000);
        board_t b3 = a & UINT64_C(0x00000000FF00FF00);
        board_t b4 = a & UINT64_C(0x0000000000FF00FF);
        return (b1 >> 32) | (b2 << 8) | (b3 >> 8) | (b4 << 32);
    }

    board_t rotate_right(board_t board)
    {
        board_t a1 = board & UINT64_C(0xF0F00000F0F00000);
        board_t a2 = board & UINT64_C(0x0F0F00000F0F0000);
        board_t a3 = board & UINT64_C(0x0000F0F00000F0F0);
        board_t a4 = board & UINT64_C(0x00000F0F00000F0F);
        board_t a = (a1 >> 4) | (a2 >> 16) | (a3 << 16) | (a4 << 4);
        board_t b1 = a & UINT64_C(0xFF00FF0000000000);
        board_t b2 = a & UINT64_C(0x00FF00FF00000000);
        board_t b3 = a & UINT64_C(0x00000000FF00FF00);
        board_t b4 = a & UINT64_C(0x0000000000FF00FF);
        return (b1 >> 8) | (b2 >> 32) | (b3 << 32) | (b4 << 8);
    }

    board_t rotate_twice(board_t board)
    {
        board_t r1 = board & UINT64_C(0xFFFF000000000000);
        board_t r2 = board & UINT64_C(0x0000FFFF00000000);
        board_t r3 = board & UINT64_C(0x00000000FFFF0000);
        board_t r4 = board & ROW_MASK;
        board = (r1 >> 48) | (r2 >> 16) | (r3 << 16) | (r4 << 48);
        board_t c1 = board & UINT64_C(0xF000F000F000F000);
        board_t c2 = board & UINT64_C(0x0F000F000F000F00);
        board_t c3 = board & UINT64_C(0x00F000F000F000F0);
        board_t c4 = board & UINT64_C(0x000F000F000F000F);
        return (c1 >> 12) | (c2 >> 4) | (c3 << 4) | (c4 << 12);
    }

    bool isLegalMove(board_t board, Move direction)
    {
        row_t r1, r2, r3, r4;
        if (direction == Move::UP || direction == Move::DOWN)
            board = transpose(board);
        r1 = (board >> 48) & ROW_MASK;
        r2 = (board >> 32) & ROW_MASK;
        r3 = (board >> 16) & ROW_MASK;
        r4 = (board >> 0) & ROW_MASK;
        if (direction == Move::LEFT || direction == Move::UP)
            return (effect_left[r1] || effect_left[r2] || effect_left[r3] || effect_left[r4]);
        return (effect_right[r1] || effect_right[r2] || effect_right[r3] || effect_right[r4]);
    }

    void performMove(board_t& board, Move direction)
    {
        board_t temp;
        switch (direction) {
        case Move::UP:
            temp = transpose(board);
            board ^= effect_up[(temp >> 48) & ROW_MASK] << 12;
            board ^= effect_up[(temp >> 32) & ROW_MASK] << 8;
            board ^= effect_up[(temp >> 16) & ROW_MASK] << 4;
            board ^= effect_up[(temp >> 0) & ROW_MASK] << 0;
            break;
        case Move::DOWN:
            temp = transpose(board);
            board ^= effect_down[(temp >> 48) & ROW_MASK] << 12;
            board ^= effect_down[(temp >> 32) & ROW_MASK] << 8;
            board ^= effect_down[(temp >> 16) & ROW_MASK] << 4;
            board ^= effect_down[(temp >> 0) & ROW_MASK] << 0;
            break;
        case Move::LEFT:
            temp = board;
            board ^= effect_left[(temp >> 48) & ROW_MASK] << 48;
            board ^= effect_left[(temp >> 32) & ROW_MASK] << 32;
            board ^= effect_left[(temp >> 16) & ROW_MASK] << 16;
            board ^= effect_left[(temp >> 0) & ROW_MASK] << 0;
            break;
        case Move::RIGHT:
            temp = board;
            board ^= effect_right[(temp >> 48) & ROW_MASK] << 48;
            board ^= effect_right[(temp >> 32) & ROW_MASK] << 32;
            board ^= effect_right[(temp >> 16) & ROW_MASK] << 16;
            board ^= effect_right[(temp >> 0) & ROW_MASK] << 0;
            break;

        case Move::RANDOM_1:
        case Move::RANDOM_2:
        case Move::UNKNOWN:
        default:
            break;
        }
    }

    void represent(board_t board, std::ostream& out)
    {
        unsigned val;
        out.flags(ios::left);
        for (int i = 0; i != 4; ++i) {
            for (int j = 0; j != 4; ++j) {
                val = GET_BOARD_UNIT(board, i, j);
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