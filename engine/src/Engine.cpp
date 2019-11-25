#include "Engine.hpp"
#include "Board.hpp"
#include "HeuristicEvaluator.hpp"
#include "IEvaluator.hpp"
#include "Player.hpp"
#include "State.hpp"

namespace game2048 {

using namespace std;

const int Engine::MAX_DEPTH = 8;
const double Engine::MIN_WEIGHT = 0.00000005;

unsigned hit_count = 0;
unsigned eval_count = 0;

Engine::Engine(IEvaluator* evaluator)
    : m_evaluator(evaluator)
#ifdef USE_CACHE
    , m_cache()
#endif
{
    if (evaluator == nullptr) {
        m_evaluator.reset(new HeuristicEvaluator());
    }
}

#ifdef USE_CACHE
bool Engine::hitCache(Node& node) const
{
    // return false;
    auto iter = m_cache.find(node.getState().getBoard());
    if (iter != m_cache.end() && iter->second.depth <= node.getDepth()) {
        node.setValue(iter->second.value);
        return true;
    }
    iter = m_cache.find(Board::transpose(node.getState().getBoard()));
    if (iter != m_cache.end() && iter->second.depth <= node.getDepth()) {
        node.setValue(iter->second.value);
        return true;
    }
    return false;
}
#endif

void Engine::_evaluate(Node& node)
{
#ifdef USE_CACHE
    if (hitCache(node)) {
        ++hit_count;
        return;
    }
#endif
    board_t board = node.getState().getBoard();
    double value = 0.0;
    if ((node.getDepth() > Engine::MAX_DEPTH) || (node.getWeight() < Engine::MIN_WEIGHT)) {
        value = m_evaluator->evaluate(board);
        node.setValue(value);
        ++eval_count;
    } else {
        node.expand();
        if (node.hasChild()) {
            if (node.getState().getPlayer() == Player::HUMAN) {
                double temp;
                for (Node& child : node.getChildren()) {
                    _evaluate(child);
                    if ((temp = child.getValue()) > value) {
                        value = temp;
                    }
                }
                node.setValue(value);
            } else {
                for (Node& child : node.getChildren()) {
                    _evaluate(child);
                }
                node.sumUpChildNodes();
                value = node.getValue();
            }
        } else {
            value = m_evaluator->evaluate(board);
            node.setValue(value);
            ++eval_count;
        }
    }

#ifdef USE_CACHE
    m_cache.insert({ board, { value, node.getDepth() } });
    m_cache.insert({ Board::rotate_left(board), { value, node.getDepth() } });
    m_cache.insert({ Board::rotate_right(board), { value, node.getDepth() } });
    m_cache.insert({ Board::rotate_twice(board), { value, node.getDepth() } });
#endif
}

double Engine::evaluate(board_t board, Player player)
{
    Node root(State(board, player), true);
    _evaluate(root);
    clog << "Hit Count  : " << hit_count << "\n"
         << "Eval Count : " << eval_count << endl;
    return root.getValue();
}

} // namespace game2048