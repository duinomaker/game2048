#include "Board.hpp"
#include "Engine.hpp"
#include "HeuristicEvaluator.hpp"
#include "Move.hpp"
#define EXPORT __declspec(dllexport)
using namespace game2048;

extern "C" {

Engine* engine;
HeuristicEvaluator* evaluator;

EXPORT void initialize_engine()
{
    Board::initialize();
    evaluator = new HeuristicEvaluator;
    evaluator->initialize();
    engine = new Engine(evaluator);
}

EXPORT void terminate_engine()
{
    delete engine;
    delete evaluator;
}

EXPORT int find_best_move(board_t board)
{
    return engine->findBestMove(board);
}

EXPORT void print_board(board_t board)
{
    Board::represent(board);
}

EXPORT board_t perform_move(board_t board, int direction)
{
    Board::performMove(board, static_cast<Move>(direction));
    return board;
}
}