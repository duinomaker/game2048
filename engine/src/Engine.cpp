#include "Engine.hpp"
#include "Board.hpp"
#include "HeuristicEvaluator.hpp"
#include "IEvaluator.hpp"
#include "Player.hpp"
#include "State.hpp"
#include <algorithm>
#include <cfloat>
#include <thread>

namespace game2048 {

using namespace std;

int Engine::MAX_DEPTH = 0;
double Engine::MIN_WEIGHT = 0.0;

#ifdef DEBUG
unsigned hit_count = 0;
unsigned eval_count = 0;
#endif

std::ostream& operator<<(std::ostream& out, const EvaluationEntry& entry)
{
    out << "{ " << entry.direction << ", " << entry.value << " }";
    return out;
}

Engine::Engine(IEvaluator* evaluator)
    : m_evaluator(evaluator)
#ifdef USE_CACHE
    , m_mutex()
    , m_cache()
#endif
{
}

#ifdef USE_CACHE
bool Engine::hitCache(Node& node)
{
    m_mutex.lock();
    ////////////////////////////////////////////////////////////////////////////////
    cache_t::iterator iter1 = m_cache.find(node.getState().getBoard());
    cache_t::iterator iter2 = m_cache.find(Board::transpose(node.getState().getBoard()));
    cache_t::iterator iter_end = m_cache.end();
    ////////////////////////////////////////////////////////////////////////////////
    m_mutex.unlock();

    if (iter1 != iter_end) {
        if (iter1->second.depth <= node.getDepth()) {
            node.setValue(iter1->second.value);
            return true;
        }
    }
    if (iter2 != iter_end && iter2->second.depth <= node.getDepth()) {
        node.setValue(iter2->second.value);
        return true;
    }
    return false;
}
#endif

void Engine::_evaluate(Node& node)
{
#ifdef USE_CACHE
    if (hitCache(node)) {
#ifdef DEBUG
        ++hit_count;
#endif
        return;
    }
#endif
    board_t board = node.getState().getBoard();
    double value = 0.0;
    if ((node.getDepth() > Engine::MAX_DEPTH) || (node.getWeight() < Engine::MIN_WEIGHT)) {
        value = m_evaluator->evaluate(board);
        node.setValue(value);
#ifdef DEBUG
        ++eval_count;
#endif
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
#ifdef DEBUG
            ++eval_count;
#endif
        }
    }

#ifdef USE_CACHE
    m_mutex.lock();
    ////////////////////////////////////////////////////////////////////////////////
    m_cache[board] = { value, node.getDepth() };
    // m_cache[Board::rotate_left(board)] = { value, node.getDepth() };
    // m_cache[Board::rotate_right(board)] = { value, node.getDepth() };
    // m_cache[Board::rotate_twice(board)] = { value, node.getDepth() };
    ////////////////////////////////////////////////////////////////////////////////
    m_mutex.unlock();
#endif
}

void Engine::evaluate(EvaluationEntry* entry)
{
    Node root(State(entry->board, Player::COMPUTER));
    _evaluate(root);
#ifdef DEBUG
    clog << "Hit Count  : " << hit_count << "\n"
         << "Eval Count : " << eval_count << endl;
#endif
    entry->value = root.getValue();
}

static void setSearchLimits(board_t board)
{
    Engine::MAX_DEPTH = 4;
}

Move Engine::findBestMove(board_t board)
{
    thread* threads[4] = { nullptr, nullptr, nullptr, nullptr };
    EvaluationEntry result[4] = {
        { 0.0, board, Move::UP },
        { 0.0, board, Move::DOWN },
        { 0.0, board, Move::LEFT },
        { 0.0, board, Move::RIGHT }
    };

    setSearchLimits(board);

    for (EvaluationEntry& entry : result) {
        if (Board::isLegalMove(entry.board, entry.direction)) {
            Board::performMove(entry.board, entry.direction);
            threads[static_cast<int>(entry.direction)]
                = new thread(&Engine::evaluate, this, &entry);
        }
    }
    for (thread* thread : threads) {
        if (thread != nullptr) {
            thread->join();
            delete thread;
        }
    }

    sort(begin(result), end(result));

#ifdef DEBUG
    clog << "{ "
         << result[0] << ", "
         << result[1] << ", "
         << result[2] << ", "
         << result[3] << " }" << endl;
#endif

    Move best_move = Move::UNKNOWN;
    double best_value = -DBL_MAX;
    for (const EvaluationEntry& entry : result) {
        if (entry.value > best_value) {
            best_move = entry.direction;
            best_value = entry.value;
        }
    }
    return best_move;
}

} // namespace game2048