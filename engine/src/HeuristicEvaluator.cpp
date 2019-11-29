#include "HeuristicEvaluator.hpp"
#include "Board.hpp"

namespace game2048 {

/** 
 * Implementation of heuristic evaluator
 *  
 * The evaluator is based mostly on the implementation of @nneonneo's,
 * but specially modified (adding two 'splitting 1024 tile' strategies)
 * for a game which stops at once when a 2048 tile is made to exploit score.
 * 
 * see https://github.com/nneonneo/2048-ai
 */

static float heur_score_table[65536];

static const float SCORE_LOST_PENALTY = 200000.0f;
static const float SCORE_MONOTONICITY_POWER = 4.0f;
static const float SCORE_MONOTONICITY_WEIGHT = 47.0f;
static const float SCORE_SUM_POWER = 3.5f;
static const float SCORE_SUM_WEIGHT = 11.0f;
static const float SCORE_MERGES_WEIGHT = 700.0f;
static const float SCORE_EMPTY_WEIGHT = 270.0f;

static const float SCORE_SPLIT1024_WEIGHT = 1000000.0f;

void HeuristicEvaluator::initialize() const
{
    for (unsigned row = 0; row < 65536; ++row) {
        unsigned line[4] = {
            (row >> 0) & 0xf,
            (row >> 4) & 0xf,
            (row >> 8) & 0xf,
            (row >> 12) & 0xf
        };

        float sum = 0;
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

        // float monotonicity_left = 0;
        // float monotonicity_right = 0;
        // for (int i = 1; i < 4; ++i) {
        //     if (line[i - 1] > line[i]) {
        //         monotonicity_left += pow(line[i - 1], SCORE_MONOTONICITY_POWER) - pow(line[i], SCORE_MONOTONICITY_POWER);
        //     } else {
        //         monotonicity_right += pow(line[i], SCORE_MONOTONICITY_POWER) - pow(line[i - 1], SCORE_MONOTONICITY_POWER);
        //     }
        // }


        // *Strategy#1* To treat 1024 tiles as 512 tiles when evaluating monotonicity.
        unsigned linew11[4];
        for (int i = 0; i != 4; ++i) {
            if (line[i] == 10)
                linew11[i] = 9;
            else
                linew11[i] = line[i];
        }

        float monotonicity_1024_left = 0.0f;
        float monotonicity_1024_right = 0.0f;
        for (int i = 1; i < 4; ++i) {
            if (linew11[i - 1] > linew11[i]) {
                monotonicity_1024_left += pow(linew11[i - 1], SCORE_MONOTONICITY_POWER) - pow(linew11[i], SCORE_MONOTONICITY_POWER);
            } else {
                monotonicity_1024_right += pow(linew11[i], SCORE_MONOTONICITY_POWER) - pow(linew11[i - 1], SCORE_MONOTONICITY_POWER);
            }
        }

        // *Strategy#2* Give higher score for lines containing two saparate 1024 tiles,
        //              or even better, three contiguous 1024 tiles.
        // for example: 1024 512  1024 128
        //           or 1024 1024 1024 256
        float split1024 = 0.0f;
        if (line[0] == 9 && line[1] == 10 && line[2] == 9 && line[3] == 10)
            split1024 += 3000000.0f;
        if (line[0] == 10 && line[1] == 9 && line[2] == 10 && line[3] == 9)
            split1024 += 3000000.0f;
        if (line[0] == 10 && line[1] == 10 && line[2] == 10)
            split1024 += 10000000.0f;
        if (line[1] == 10 && line[2] == 10 && line[3] == 10)
            split1024 += 10000000.0f;
        if (line[0] == 10 && line[2] == 10 && line[3] == 10)
            split1024 += 10000000.0f;
        if (line[0] == 10 && line[1] == 10 && line[3] == 10)
            split1024 += 10000000.0f;
        if (line[0] == 10 && line[1] && line[1] != 10 && line[2] == 10)
            split1024 += 2.0f;
        if (line[1] == 10 && line[2] && line[2] != 10 && line[3] == 10)
            split1024 += 2.0f;
        if (line[0] == 10 && ((line[1] && line[1] != 10) || (line[2] && line[2] != 10)) && line[3] == 10)
            split1024 += 1.0f;

        heur_score_table[row]
            = SCORE_LOST_PENALTY
            + SCORE_EMPTY_WEIGHT * empty
            + SCORE_MERGES_WEIGHT * merges
            // - SCORE_MONOTONICITY_WEIGHT * std::min(monotonicity_left, monotonicity_right)
            - SCORE_MONOTONICITY_WEIGHT * std::min(monotonicity_1024_left, monotonicity_1024_right)
            - SCORE_SUM_WEIGHT * sum
            + SCORE_SPLIT1024_WEIGHT * split1024;
    }
}

static float score_helper(board_t board)
{
    return heur_score_table[(board >> 0) & ROW_MASK]
        + heur_score_table[(board >> 16) & ROW_MASK]
        + heur_score_table[(board >> 32) & ROW_MASK]
        + heur_score_table[(board >> 48) & ROW_MASK];
}

float HeuristicEvaluator::evaluate(board_t board) const
{
    return score_helper(board) + score_helper(Board::transpose(board));
}

} // namespace 2048