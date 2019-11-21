#include "Board.hpp"
#include "Node.hpp"
#include "Player.hpp"
#include "State.hpp"
#include <cstdint>
#include <iostream>

using namespace std;
using namespace game2048;

int main(int, char**)
{
    int arr[4][4] = {
        { 0, 0, 0, 0 },
        { 1, 7, 5, 7 },
        { 6, 0, 0, 9 },
        { 0, 0, 4, 0 }
    };

    Node node(Board(arr), 0, Player::COMPUTER);
    node.expand();

    cout << boolalpha;
    cout << node.hasChild() << endl;

    for (auto& child : node.getChildren()) {
        child.getState().getBoard().print();
        cout << "################################\n"
             << endl;
    }

    return 0;
}