#ifndef __GAME2048_PLAYER__
#define __GAME2048_PLAYER__
#include <iostream>

namespace game2048 {

enum class Player {
    HUMAN = +1,
    COMPUTER = -1,
    UNKNOWN = 0
};

inline Player opponentOf(Player player)
{
    return static_cast<Player>(-static_cast<int>(player));
}

std::ostream& operator<<(std::ostream& out, Player player);

} // namespace game2048

#endif // __GAME2048_PLAYER__