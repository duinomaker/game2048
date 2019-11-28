#include "HeuristicEvaluator.hpp"
#include "Board.hpp"

namespace game2048 {

/** 
 *  Implementation of heuristic evaluator
 * 
 *  see https://github.com/nneonneo/2048-ai
 */

static double heur_score_table[65536];
static double score_table[65536];

static const double SCORE_LOST_PENALTY = 200000.0;
static const double SCORE_MONOTONICITY_POWER = 4.0;
static const double SCORE_MONOTONICITY_WEIGHT = 47.0;
static const double SCORE_SUM_POWER = 3.5;
static const double SCORE_SUM_WEIGHT = 11.0;
static const double SCORE_MERGES_WEIGHT = 700.0;
static const double SCORE_EMPTY_WEIGHT = 270.0;

void HeuristicEvaluator::initialize() const
{
    for (unsigned row = 0; row < 65536; ++row) {
        unsigned line[4] = {
            (row >> 0) & 0xf,
            (row >> 4) & 0xf,
            (row >> 8) & 0xf,
            (row >> 12) & 0xf
        };

        // Score
        double score = 0.0;
        for (int i = 0; i < 4; ++i) {
            unsigned rank = line[i];
            if (rank >= 2) {
                score += (rank - 1) * (1 << rank);
            }
        }
        score_table[row] = score;

        // Heuristic score
        double sum = 0;
        int empty = 0;
        int merges = 0;

        int prev = 0;
        int counter = 0;
        for (int i = 0; i < 4; ++i) {
            unsigned rank = line[i];
            sum += pow(rank, SCORE_SUM_POWER);
            if (rank == 0) {
                ++empty;
            } else {
                if (prev == rank) {
                    ++counter;
                } else if (counter > 0) {
                    merges += 1 + counter;
                    counter = 0;
                }
                prev = rank;
            }
        }
        if (counter > 0) {
            merges += 1 + counter;
        }

        double monotonicity_left = 0;
        double monotonicity_right = 0;
        for (int i = 1; i < 4; ++i) {
            if (line[i - 1] > line[i]) {
                monotonicity_left += pow(line[i - 1], SCORE_MONOTONICITY_POWER) - pow(line[i], SCORE_MONOTONICITY_POWER);
            } else {
                monotonicity_right += pow(line[i], SCORE_MONOTONICITY_POWER) - pow(line[i - 1], SCORE_MONOTONICITY_POWER);
            }
        }

        heur_score_table[row] = SCORE_LOST_PENALTY
            + SCORE_EMPTY_WEIGHT * empty
            + SCORE_MERGES_WEIGHT * merges
            - SCORE_MONOTONICITY_WEIGHT * std::min(monotonicity_left, monotonicity_right)
            - SCORE_SUM_WEIGHT * sum;
    }
}

static double score_helper(board_t board)
{
    double sum = 0.0;
    sum += heur_score_table[(board >> 0) & ROW_MASK]
        + heur_score_table[(board >> 16) & ROW_MASK]
        + heur_score_table[(board >> 32) & ROW_MASK]
        + heur_score_table[(board >> 48) & ROW_MASK];
    return sum;
}

double HeuristicEvaluator::evaluate(board_t board) const
{
    return score_helper(board) + score_helper(Board::transpose(board));
}

} // namespace 2048