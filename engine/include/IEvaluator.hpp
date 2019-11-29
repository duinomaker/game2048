#ifndef __GAME2048_IEVALUATOR__
#define __GAME2048_IEVALUATOR__
#include "Board.hpp"

namespace game2048 {

class IEvaluator {
public:
    virtual ~IEvaluator() {}

    virtual float evaluate(board_t board) const = 0;
};

} // namespace game2048

#endif // __GAME2048_IEVALUATOR__