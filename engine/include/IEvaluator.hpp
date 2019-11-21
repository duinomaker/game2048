#ifndef __GAME2048_IEVALUATOR__
#define __GAME2048_IEVALUATOR__
#include "Node.hpp"

namespace game2048 {

class IEvaluator {
public:
    virtual ~IEvaluator() {}

    virtual double evaluate(const Node& node) const = 0;
};

} // namespace game2048

#endif // __GAME2048_IEVALUATOR__