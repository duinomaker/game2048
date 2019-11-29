#include "Board.hpp"
#include "Engine.hpp"
#include "HeuristicEvaluator.hpp"
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;
using namespace game2048;

int main(int, char**)
{
    Board::initialize();
    HeuristicEvaluator evaluator;
    evaluator.initialize();

    int arr[4][4] = {
        { 0, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 0 }
    };

    Engine engine(&evaluator);

    steady_clock::time_point t1 = steady_clock::now();
    clog << "Best Move  : " << engine.findBestMove(Board::fromArray(arr)) << endl;
    steady_clock::time_point t2 = steady_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout << "It took " << time_span.count() << " seconds.";
}