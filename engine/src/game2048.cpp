#include "Board.hpp"
#include "Move.hpp"
#include "Node.hpp"
#include "Player.hpp"
#include "State.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>
#include <utility>

using namespace std;
using namespace game2048;

int main(int, char**)
{
    int arr_raw[4][4] = {
        { 11, 10, 4, 0 },
        { 12, 3 , 4, 0 },
        { 0 , 3 , 4, 0 },
        { 0 , 0 , 4, 5 }
    };

    State state(Board::fromArray(arr_raw), Player::HUMAN);
    Node node(state);

    clog << node << endl;

    return 0;
}