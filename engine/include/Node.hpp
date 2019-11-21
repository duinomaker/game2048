#ifndef __GAME2048_NODE__
#define __GAME2048_NODE__
#include "Board.hpp"
#include "Player.hpp"
#include "State.hpp"
#include <cstddef>
#include <list>

namespace game2048 {

class Node {
private:
    double m_prob;
    std::size_t m_size;
    State m_state;
    std::list<Node> m_children;

public:
    Node();

    Node(const State& state);

    Node(const Board& board, int score, Player player);

    void reset();

    void reset(const Board& board, int score, Player player);

    bool hasChild() const { return m_size != 0; }

    std::list<Node>& getChildren() { return m_children; }

    State getState() { return m_state; }

    std::size_t expand();
};

} // namespace game2048

#endif // __GAME2048_NODE__