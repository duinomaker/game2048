#ifndef __GAME2048_DIRECTION__
#define __GAME2048_DIRECTION__

namespace game2048 {

enum class Direction {
    UNKNOWN = -1,
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

static const Direction allDirections[] = {
    Direction::UP,
    Direction::DOWN,
    Direction::LEFT,
    Direction::RIGHT
};

} // namespace game2048

#endif // __GAME2048_DIRECTION__