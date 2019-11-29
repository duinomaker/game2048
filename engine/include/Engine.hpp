#ifndef __GAME2048_ENGINE__
#define __GAME2048_ENGINE__
#include "Board.hpp"
#include "IEvaluator.hpp"
#include "Move.hpp"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <unordered_map>

namespace game2048 {

struct CacheEntry {
    float value;
    std::uint32_t depth;
};

using cache_t = std::unordered_map<board_t, CacheEntry>;

struct EvaluationEntry {
    float value;
    Move direction;
    board_t board;

    bool operator<(const EvaluationEntry& rhs) { return value > rhs.value; }
};

std::ostream& operator<<(std::ostream& out, const EvaluationEntry& entry);

class Engine {
private:
    IEvaluator* m_evaluator;
    std::mutex m_mutex;
    cache_t m_cache;

private:
    float hitCache(std::uint32_t depth, board_t board);

    float evaluate_human_node(std::uint32_t depth, std::uint32_t max_depth, board_t board, float weight);

    float evaluate_computer_node(std::uint32_t depth, std::uint32_t max_depth, board_t board, float weight);

    void evaluate(EvaluationEntry* entry);

public:
    explicit Engine(IEvaluator* evaluator = nullptr);

    int findBestMove(board_t board);
};

} // namespace game2048

#endif // __GAME2048_ENGINE__