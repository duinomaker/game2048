#ifndef __GAME2048_NODE__
#define __GAME2048_NODE__
#include "State.hpp"
#include <cstddef>
#include <list>

namespace game2048 {

class Node {
    friend std::ostream& operator<<(std::ostream& out, const Node& node);

private:
    double m_weight;
    double m_value;
    std::size_t m_size;
    State m_state;
    std::list<Node> m_children;

public:
    Node(const State& state = 0);

    bool hasChild() const { return m_size != 0; }

    const State getState() const { return m_state; }

    std::list<Node>& getChildren() { return m_children; }

    std::size_t expand();
};

} // namespace game2048

#endif // __GAME2048_NODE__