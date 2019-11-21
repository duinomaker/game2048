#include "Player.hpp"
#include <iostream>

namespace game2048 {

using namespace std;

std::ostream& operator<<(std::ostream& out, Player player)
{
    switch (player) {
    case Player::HUMAN:
        out << "HUMAN";
        break;
    case Player::COMPUTER:
        out << "COMPUTER";
        break;

    case Player::UNKNOWN:
    default:
        out << "UNKNOWN";
        break;
    }
    return out;
}

} // namespace game2048