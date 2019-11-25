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
    int m_depth;
    std::size_t m_size;
    State m_state;
    std::list<Node> m_children;

public:
    Node(const State& state = 0, bool isRoot = false);

    bool hasChild() const { return m_size != 0; }

    double getWeight() const { return m_weight; }

    double getValue() const { return m_value / m_weight; }

    void setValue(double value) { m_value = m_weight * value; }

    int getDepth() const { return m_depth; }

    const State getState() const { return m_state; }

    std::list<Node>& getChildren() { return m_children; }

    const std::list<Node>& getChildren() const { return m_children; }

    std::size_t expand();

    void sumUpChildNodes();
};

} // namespace game2048

#endif // __GAME2048_NODE__