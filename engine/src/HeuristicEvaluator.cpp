#include "HeuristicEvaluator.hpp"
#include "Board.hpp"

namespace game2048 {

double HeuristicEvaluator::evaluate(board_t board) const
{
    // not implemented yet, this is just a prototype
    int value = 0;
    unsigned temp;
    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            if (temp = GET_BOARD_UNIT(board, i, j))
                value += 1 << temp;
    return static_cast<double>(value);
}

} // namespace 2048