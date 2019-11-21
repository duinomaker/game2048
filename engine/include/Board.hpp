#ifndef __GAME2048_BOARD__
#define __GAME2048_BOARD__
#include "Direction.hpp"
#include <cstdint>
#include <list>

#define _TO_OFFSET(row, col) ((row << 4) | (col << 2))
#define GET_UNIT(board, row, col) ((board >> _TO_OFFSET(row, col)) & UINT64_C(0xf))
#define SET_UNIT(board, row, col, val) (board ^= ((GET_UNIT(board, row, col) ^ val) << _TO_OFFSET(row, col)))

namespace game2048 {

class Board {
private:
    Direction m_lastMove;
    std::uint64_t m_board;

public:
    Board(std::uint64_t board = 0);

    Board(const Board& board);

    Board(int board[4][4]);

    void reset(std::uint64_t board = 0);

    void reset(int board[4][4]);

    void reset(const Board& board);

    void toArray(int board[4][4]) const;

    void print() const;

    bool isLegalMove(Direction direction) const;

    int performMove(Direction direction);

    std::list<Board> getComputerMovedBoards() const;
};

} // namespace game2048

#endif // __GAME2048_BOARD__