#ifndef __GAME2048_BOARD__
#define __GAME2048_BOARD__
#include "Move.hpp"
#include <array>
#include <cstdint>
#include <iostream>

#define ROW_MASK UINT64_C(0x000000000000FFFF)

#define _TO_OFFSET(row, col) ((row << 4) | (col << 2))
#define GET_BOARD_UNIT(board, row, col) ((board >> _TO_OFFSET(row, col)) & UINT64_C(0x000000000000000F))
#define SET_BOARD_UNIT(board, row, col, val) (board ^= ((GET_BOARD_UNIT(board, row, col) ^ val) << _TO_OFFSET(row, col)))
#define GET_ROW_UNIT(row, col) ((row >> (col << 2)) & UINT16_C(0x000F))
#define SET_ROW_UNIT(row, col, val) (row ^= (GET_ROW_UNIT(row, col) ^ val) << (col << 2))

namespace game2048 {

using board_t = std::uint64_t;
using row_t = std::uint16_t;

namespace Board {

    extern std::array<board_t, 65536> effect_up;
    extern std::array<board_t, 65536> effect_down;
    extern std::array<board_t, 65536> effect_left;
    extern std::array<board_t, 65536> effect_right;
    extern std::array<int, 65536> bitset_ones;

    void initialize();

    board_t fromArray(int arr[][4]);

    void toArray(board_t board, int arr[][4]);

    board_t transpose(board_t board);

    int countEmpty(board_t board);

    int countDistinctTiles(board_t board);

    bool isLegalMove(board_t board, Move direction);

    void performMove(board_t& board, Move direction);

    void represent(board_t board, std::ostream& out = std::clog);

    static void _performRowMove(row_t& row, Move direction);

    static board_t _toColumn(board_t row);

} // namespace Board

} // namespace game2048

#endif // __GAME2048_BOARD__