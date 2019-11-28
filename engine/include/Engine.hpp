#ifndef __GAME2048_ENGINE__
#define __GAME2048_ENGINE__
#include "Board.hpp"
#include "IEvaluator.hpp"
#include "Move.hpp"
#include "Node.hpp"
#include "Player.hpp"
#include <iostream>
#include <memory>
#ifdef USE_CACHE
#include <mutex>
#include <unordered_map>
#endif

namespace game2048 {

struct EvaluationEntry {
    double value;
    board_t board;
    Move direction;

    bool operator<(const EvaluationEntry& rhs) { return value > rhs.value; }
};

std::ostream& operator<<(std::ostream& out, const EvaluationEntry& entry);

#ifdef USE_CACHE
struct CacheEntry {
    double value;
    int depth;
};
using cache_t = std::unordered_map<board_t, CacheEntry>;
#endif

class Engine {
public:
    static int MAX_DEPTH;
    static double MIN_WEIGHT;

private:
    IEvaluator* m_evaluator;
#ifdef USE_CACHE
    std::mutex m_mutex;
    cache_t m_cache;
#endif


public:
    explicit Engine(IEvaluator* evaluator = nullptr);
#ifdef USE_CACHE
    bool hitCache(Node& node);
#endif
    void _evaluate(Node& node);

    void evaluate(EvaluationEntry* entry);

    Move findBestMove(board_t board);
};

} // namespace game2048

#endif // __GAME2048_ENGINE__