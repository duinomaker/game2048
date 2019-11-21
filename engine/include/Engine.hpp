#ifndef __GAME2048_ENGINE__
#define __GAME2048_ENGINE__
#include "IEvaluator.hpp"
#include "Node.hpp"
#include <memory>
#ifdef USE_CACHE
#include <unordered_map>
#endif

namespace game2048 {

class Engine {
public:
    static const int DEFAULT_LEVEL = 3;

private:
    int m_level;
    Node m_root;
    std::unique_ptr<IEvaluator> m_evaluator;
#ifdef USE_CACHE
    std::unordered_map<uint64_t, double> m_cache;
#endif

public:
    explicit Engine(IEvaluator* evaluator = nullptr);

#ifdef USE_CACHE
    bool hitCache(const Node& node) const;
#endif

    double evaluate(const Node& node) const;
};

} // namespace game2048

#endif // __GAME2048_ENGINE__