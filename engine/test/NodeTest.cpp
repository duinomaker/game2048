#include "Node.hpp"
#include "Board.hpp"
#include "HeuristicEvaluator.hpp"
#include "Move.hpp"
#include "Player.hpp"
#include "SimpleTest.hpp"
#include "State.hpp"
#include <iostream>

using namespace std;
using namespace game2048;

INIT_TEST(NodeTest)

HeuristicEvaluator evaluator;

TEST(expand_weigh_sumup)
{
    int arr[4][4] = {
        { 4, 3, 2, 1 },
        { 3, 2, 1, 0 },
        { 2, 1, 0, 0 },
        { 1, 0, 0, 0 }
    };

    State state(Board::fromArray(arr), Player::HUMAN);
    Node root(state, true);

    root.expand();
    unsigned count = 0;

    for (Node& i : root.getChildren()) {
        i.expand();
        for (Node& j : i.getChildren()) {
            j.expand();
            for (Node& k : j.getChildren()) {
                k.expand();
                for (Node& l : k.getChildren()) {
                    l.setValue(1.0);
                    ++count;
                }
                k.sumUpChildNodes();
            }
            j.sumUpChildNodes();
        }
        i.sumUpChildNodes();
    }
    root.sumUpChildNodes();

    clog << root << endl;
    clog << count << " Leaf Nodes" << endl;

    EXPECT_DOUBLE_EQ(root.getValue(), 1.0);
}
ENDTEST()

TERMINATE_TEST()