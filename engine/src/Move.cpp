#include "Move.hpp"
#include <iostream>

namespace game2048 {

using namespace std;

ostream& operator<<(ostream& out, Move mov)
{
    switch (mov) {
    case Move::UP:
        out << "UP";
        break;
    case Move::DOWN:
        out << "DOWN";
        break;
    case Move::LEFT:
        out << "LEFT";
        break;
    case Move::RIGHT:
        out << "RIGHT";
        break;
    case Move::COMPUTER_1:
        out << "RANDOM_1";
        break;
    case Move::COMPUTER_2:
        out << "RANDOM_2";
        break;

    case Move::UNKNOWN:
    default:
        out << "UNKNOWN";
        break;
    }
    return out;
}

} // namespace game2048