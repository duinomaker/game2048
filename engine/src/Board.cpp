#include "Board.hpp"
#include "Move.hpp"
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

namespace game2048 {

using namespace std;

namespace Board {

    array<board_t, 65536> effect_up;
    array<board_t, 65536> effect_down;
    array<board_t, 65536> effect_left;
    array<board_t, 65536> effect_right;
    array<int, 65536> bitset_ones;

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

        int count;
        uint16_t temp;
        for (uint16_t bitset = 1; bitset != 0; ++bitset) {
            count = 0;
            temp = bitset;
            while (temp) {
                temp &= temp - 1;
                ++count;
            }
            bitset_ones[bitset] = count;
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

    int countEmpty(board_t board)
    {
        board |= (board >> 2) & UINT64_C(0x3333333333333333);
        board |= (board >> 1);
        board = ~board & UINT64_C(0x1111111111111111);
        board += board >> 32;
        board += board >> 16;
        board += board >> 8;
        board += board >> 4;
        return board & 0xF;
    }

    int countDistinctTiles(board_t board)
    {
        uint16_t bitset = 0;
        while (board) {
            bitset |= 1 << (board & UINT64_C(0xF));
            board >>= 4;
        }
        return bitset_ones[bitset >> 1];
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
            return effect_left[r1] || effect_left[r2] || effect_left[r3] || effect_left[r4];
        return effect_right[r1] || effect_right[r2] || effect_right[r3] || effect_right[r4];
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

    static void _performRowMove(row_t& row, Move direction)
    {
        row_t row_new = 0;
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
        return (((row >> 0) & UINT64_C(0xF)) << 0)
            | (((row >> 4) & UINT64_C(0xF)) << 16)
            | (((row >> 8) & UINT64_C(0xF)) << 32)
            | (((row >> 12) & UINT64_C(0xF)) << 48);
    }

} // namespace Board

} // namespace game2048