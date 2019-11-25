#ifndef __GAME2048_ENGINE__
#define __GAME2048_ENGINE__
#include "Board.hpp"
#include "IEvaluator.hpp"
#include "Node.hpp"
#include "Player.hpp"
#include <memory>
#ifdef USE_CACHE
#include <unordered_map>
#endif

namespace game2048 {

#ifdef USE_CACHE
struct CacheEntry {
    double value;
    int depth;
};
#endif

class Engine {
public:
    static const int MAX_DEPTH;
    static const double MIN_WEIGHT;

private:
    std::unique_ptr<IEvaluator> m_evaluator;
#ifdef USE_CACHE
    std::unordered_map<board_t, CacheEntry> m_cache;
#endif

public:
    explicit Engine(IEvaluator* evaluator = nullptr);

#ifdef USE_CACHE
    bool hitCache(Node& node) const;
#endif

    void _evaluate(Node& node);

    double evaluate(board_t board, Player player);
};

} // namespace game2048

#endif // __GAME2048_ENGINE__