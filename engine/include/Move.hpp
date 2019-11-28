#ifndef __GAME2048_MOVE__
#define __GAME2048_MOVE__
#include <iostream>

namespace game2048 {

enum class Move {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    RANDOM_1 = 4,
    RANDOM_2 = 5,
    UNKNOWN = -1
};

static const Move allDirections[] = {
    Move::UP,
    Move::DOWN,
    Move::LEFT,
    Move::RIGHT
};

std::ostream& operator<<(std::ostream& out, Move mov);

} // namespace game2048

#endif // __GAME2048_MOVE__