#ifndef __GAME2048_BOARD__
#define __GAME2048_BOARD__
#include "Move.hpp"
#include <cstdint>
#include <iostream>
#include <list>
#include <utility>

#define _TO_OFFSET(row, col) ((row << 4) | (col << 2))
#define GET_UNIT(board, row, col) ((board >> _TO_OFFSET(row, col)) & UINT64_C(0xf))
#define SET_UNIT(board, row, col, val) (board ^= ((GET_UNIT(board, row, col) ^ val) << _TO_OFFSET(row, col)))

namespace game2048 {

namespace Board {

    std::uint64_t fromArray(int arr[][4]);

    void toArray(std::uint64_t board, int arr[][4]);

    bool isLegalMove(std::uint64_t board, Move direction);

    int performMove(std::uint64_t& board, Move direction);

    void performMove(std::uint64_t& board, Move direction, const std::pair<int, int>& coord);

    std::list<std::pair<int, int>> getAllSlots(std::uint64_t board);

    void represent(std::uint64_t board, std::ostream& out = std::clog);

} // namespace Board

} // namespace game2048

#endif // __GAME2048_BOARD__