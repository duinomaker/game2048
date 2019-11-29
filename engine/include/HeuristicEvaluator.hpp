#ifndef __GAME2048_HEURISTICEVALUATOR__
#define __GAME2048_HEURISTICEVALUATOR__
#include "Board.hpp"
#include "IEvaluator.hpp"

namespace game2048 {

class HeuristicEvaluator : public IEvaluator {
public:
    void initialize() const;

    float evaluate(board_t board) const;
};

} // namespace game2048

#endif // __GAME2048_HEURISTICEVALUATOR__