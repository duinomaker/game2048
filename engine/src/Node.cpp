#include "Node.hpp"
#include "Player.hpp"
#include "State.hpp"
#include "Move.hpp"
#include <cstddef>
#include <list>

namespace game2048 {

using namespace std;

Node::Node(const State& state)
    : m_weight(1.0)
    , m_value(0.0)
    , m_depth(0)
    , m_state(state)
    , m_children()
{
}

void Node::expand()
{
    double temp;
    if (m_state.getPlayer() == Player::HUMAN) {
        for (const State& state : m_state.getAllSubStates())
            m_children.emplace_back(state);
        temp = static_cast<double>(m_children.size());
        for (Node& child : m_children) {
            child.m_weight = m_weight / temp;
            child.m_depth = m_depth + 1;
        }
    } else {
        for (const State& state : m_state.getAllSubStates())
            m_children.emplace_back(state);
        temp = static_cast<double>(m_children.size() + m_children.size() / 2);
        for (Node& child : m_children) {
            child.m_weight = (child.getState().getLastMove() == Move::RANDOM_1 ? 2.0 : 1.0) * m_weight / temp;
            child.m_depth = m_depth + 1;
        }
    }
}

void Node::sumUpChildNodes()
{
    m_value = 0;
    for (const Node& child : m_children)
        m_value += child.m_value;
}

std::ostream& operator<<(std::ostream& out, const Node& node)
{
    out << node.m_state
        << "\n* Depth     : " << node.m_depth
        << "\n* Children  : " << node.m_children.size()
        << "\n* Weight    : " << node.m_weight
        << "\n* Value     : " << node.m_value;
    return out;
}

} // namespace game2048