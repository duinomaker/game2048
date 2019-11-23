#ifndef __GAME2048_MOVE__
#define __GAME2048_MOVE__
#include <iostream>

namespace game2048 {

enum class Move {
    UNKNOWN = 0,
    COMPUTER_1 = 1,
    COMPUTER_2 = 2,
    UP = 3,
    DOWN = 4,
    LEFT = 5,
    RIGHT = 6
};

static const Move allDirections[] = {
    Move::UP,
    Move::DOWN,
    Move::LEFT,
    Move::RIGHT
};

static const Move allComputerMoves[] = {
    Move::COMPUTER_1,
    Move::COMPUTER_2
};

std::ostream& operator<<(std::ostream& out, Move mov);

} // namespace game2048

#endif // __GAME2048_MOVE__