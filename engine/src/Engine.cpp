#include "Engine.hpp"
#include "HeuristicEvaluator.hpp"
#include "IEvaluator.hpp"

namespace game2048 {

Engine::Engine(IEvaluator* evaluator)
    : m_level(Engine::DEFAULT_LEVEL)
    , m_root()
    , m_evaluator()
#ifdef USE_CACHE
    , m_cache()
#endif
{
    if (evaluator == nullptr) {
        m_evaluator.reset(new HeuristicEvaluator());
    }
}

#ifdef USE_CACHE
bool Engine::hitCache(const Node& node) const
{
    // not implemented yet
    return false;
}
#endif

double Engine::evaluate(const Node& node) const
{
    return m_evaluator->evaluate(node);
}

} // namespace game2048