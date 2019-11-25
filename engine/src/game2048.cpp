#include "Board.hpp"
#include "Engine.hpp"
#include "HeuristicEvaluator.hpp"
#include "Move.hpp"
#include "Node.hpp"
#include "Player.hpp"
#include "State.hpp"
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <unordered_map>
#include <utility>

using namespace std;
using namespace std::chrono;
using namespace game2048;

int main(int, char**)
{
    Board::initialize();

    // int arr[4][4] = {
    //     { 1, 1, 1, 1 },
    //     { 2, 2, 2, 0 },
    //     { 3, 3, 0, 0 },
    //     { 4, 0, 0, 0 }
    // };
    // board_t board = Board::transpose_sub(Board::fromArray(arr));

    int arr[4][4] = {
        { 0, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 0 }
    };

    Engine engine(new HeuristicEvaluator);

    steady_clock::time_point t1 = steady_clock::now();
    clog << "Value      : " << engine.evaluate(Board::fromArray(arr), Player::HUMAN) << endl;
    steady_clock::time_point t2 = steady_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout << "It took " << time_span.count() << " seconds.";
}