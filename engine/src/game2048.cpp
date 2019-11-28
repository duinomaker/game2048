#include "Board.hpp"
#include "Engine.hpp"
#include "HeuristicEvaluator.hpp"
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;
using namespace game2048;

// extern "C" {

// HeuristicEvaluator* evaluator;
// Engine* engine;

// __declspec(dllexport) void initialize_engine()
// {
//     Board::initialize();
//     evaluator = new HeuristicEvaluator;
//     evaluator->initialize();
//     engine = new Engine(evaluator);
// }

// __declspec(dllexport) int findBestMove(board_t board)
// {
//     return engine->findBestMove(board);
// }

// __declspec(dllexport) void terminate_engine()
// {
//     delete engine;
//     delete evaluator;
// }
// }

#ifdef DEBUG
int main(int, char**)
{
    // unordered_map<int, int> ump;

    // ump.insert({ 576, 8054 });
    // // ump.erase(576);
    // ump.insert({ 576, 8192 });

    // cout << ump[576] << endl;

    Board::initialize();
    HeuristicEvaluator evaluator;
    evaluator.initialize();

    int arr[4][4] = {
        { 10, 10, 0, 0 },
        { 2, 3, 9, 8 },
        { 0, 2, 0, 8 },
        { 0, 0, 0, 0 }
    };

    Engine engine(&evaluator);

    steady_clock::time_point t1 = steady_clock::now();
    clog << "Best Move  : " << engine.findBestMove(Board::fromArray(arr)) << endl;
    steady_clock::time_point t2 = steady_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout << "It took " << time_span.count() << " seconds.";
}
#endif