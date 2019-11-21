#ifndef __GAME2048_HEURISTICEVALUATOR__
#define __GAME2048_HEURISTICEVALUATOR__
#include "IEvaluator.hpp"
#include "State.hpp"

namespace game2048 {

class HeuristicEvaluator : public IEvaluator {
public:
    double evaluate(const Node& node) const;
};

} // namespace game2048

#endif // __GAME2048_HEURISTICEVALUATOR__