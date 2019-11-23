#include "Node.hpp"
#include "Player.hpp"
#include "State.hpp"
#include <cstddef>
#include <list>

namespace game2048 {

using namespace std;

Node::Node(const State& state)
    : m_weight(0.0)
    , m_value(0.0)
    , m_size(0)
    , m_state(state)
    , m_children()
{
}

size_t Node::expand()
{
    m_size = 0;
    switch (m_state.getPlayer()) {
        double temp;
        for (const State& state : m_state.getAllSubStates()) {
            m_children.emplace_back(state);
        }
        m_size = m_children.size();
    case Player::HUMAN:
        temp = static_cast<double>(m_size);
        for (Node& child : m_children) {
            child.m_weight = 1.0 / temp;
        }
        break;
    case Player::COMPUTER:
        temp = static_cast<double>(m_size + m_size / 2);
        for (Node& child : m_children) {
            child.m_weight = static_cast<double>(child.getState().getLastMove()) / temp;
        }
        break;

    case Player::UNKNOWN:
    default:
        break;
    }
    return m_size;
}

std::ostream& operator<<(std::ostream& out, const Node& node)
{
    out << node.m_state
        << "\n* Children  : " << node.m_size
        << "\n* Weight    : " << node.m_weight
        << "\n* Value     : " << node.m_value;

    return out;
}

} // namespace game2048