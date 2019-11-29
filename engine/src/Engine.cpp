#include "Engine.hpp"
#include "Board.hpp"
#include "IEvaluator.hpp"
#include <cfloat>
#include <cstdint>
#include <iostream>
#include <thread>
#ifdef DEBUG
#include <algorithm>
#endif

namespace game2048 {

using namespace std;

Engine::Engine(IEvaluator* evaluator)
    : m_evaluator(evaluator)
    , m_mutex()
    , m_cache()
{
}

float Engine::hitCache(uint32_t depth, board_t board)
{
    m_mutex.lock();
    ////////////////////////////////////////////////////////////////////////////////
    cache_t::iterator iter1 = m_cache.find(board);
    // cache_t::iterator iter2 = m_cache.find(Board::transpose(board));
    cache_t::iterator iter_end = m_cache.end();
    ////////////////////////////////////////////////////////////////////////////////
    m_mutex.unlock();
    if (iter1 != iter_end && iter1->second.depth <= depth) {
        return iter1->second.value;
    }
    // if (iter2 != iter_end && iter2->second.depth <= depth) {
    //     return iter2->second.value;
    // }
    return 0.0f;
}

float Engine::evaluate_human_node(uint32_t depth, uint32_t max_depth, board_t board, float weight)
{
    // float temp = hitCache(depth, board);
    // if (temp) {
    //     return temp;
    // }
    float temp;
    float value_max = -FLT_MAX;
    board_t board_new;
    for (Move direction : allDirections) {
        if (Board::isLegalMove(board, direction)) {
            Board::performMove(board_new = board, direction);
            // if ((temp = hitCache(depth, board)) == 0.0f) {
            //     temp = evaluate_computer_node(depth + 1, max_depth, board_new, weight);
            // }
            temp = evaluate_computer_node(depth + 1, max_depth, board_new, weight);
            if (temp > value_max) {
                value_max = temp;
            }
        }
    }
    // m_mutex.lock();
    // ////////////////////////////////////////////////////////////////////////////////
    // m_cache[board] = { value_max, depth };
    // ////////////////////////////////////////////////////////////////////////////////
    // m_mutex.unlock();
    return value_max;
}

float Engine::evaluate_computer_node(uint32_t depth, uint32_t max_depth, board_t board, float weight)
{
    float value_mean = 0.0f;
    if (depth > max_depth || weight < 0.0001f) {
        value_mean = m_evaluator->evaluate(board);
    } else {
        float temp = hitCache(depth, board);
        if (temp) {
            return temp;
        }
        float child_count = static_cast<float>(Board::countEmpty(board));
        weight /= child_count;
        for (int offset = 0; offset != 64; offset += 4) {
            if (!((board >> offset) & 0xF)) {
                value_mean += evaluate_human_node(depth + 1, max_depth, board | (UINT64_C(1) << offset), weight * 0.9f) * 0.9f;
                value_mean += evaluate_human_node(depth + 1, max_depth, board | (UINT64_C(2) << offset), weight * 0.1f) * 0.1f;
            }
        }
        value_mean /= child_count;
    }
    m_mutex.lock();
    ////////////////////////////////////////////////////////////////////////////////
    m_cache[board] = { value_mean, depth };
    ////////////////////////////////////////////////////////////////////////////////
    m_mutex.unlock();
    return value_mean;
}

void Engine::evaluate(EvaluationEntry* entry)
{
    uint32_t max_depth = static_cast<uint32_t>(max(5, Board::countDistinctTiles(entry->board) - 2));
    entry->value = evaluate_computer_node(0, max_depth, entry->board, 1.0f);
}

int Engine::findBestMove(board_t board)
{
    thread* threads[4] = { nullptr, nullptr, nullptr, nullptr };
    EvaluationEntry result[4] = {
        { 0.0, Move::UP, board },
        { 0.0, Move::DOWN, board },
        { 0.0, Move::LEFT, board },
        { 0.0, Move::RIGHT, board }
    };
    for (EvaluationEntry& entry : result) {
        if (Board::isLegalMove(entry.board, entry.direction)) {
            Board::performMove(entry.board, entry.direction);
            threads[static_cast<int>(entry.direction)]
                = new thread(&Engine::evaluate, this, &entry);
        }
    }
    for (thread* thread : threads) {
        if (thread != nullptr) {
            thread->join();
            delete thread;
        }
    }

#ifdef DEBUG
    sort(begin(result), end(result));

    clog << "{ "
         << result[0] << ", "
         << result[1] << ", "
         << result[2] << ", "
         << result[3] << " }" << endl;
#endif

    Move best_move = Move::UNKNOWN;
    float best_value = -FLT_MAX;
    for (const EvaluationEntry& entry : result) {
        if (entry.value > best_value) {
            best_move = entry.direction;
            best_value = entry.value;
        }
    }
    return static_cast<int>(best_move);
}

std::ostream& operator<<(std::ostream& out, const EvaluationEntry& entry)
{
    out << "{ " << entry.direction << ", " << entry.value << " }";
    return out;
}

} // namespace game2048