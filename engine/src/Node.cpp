#include "Node.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Player.hpp"
#include "State.hpp"

namespace game2048 {

using namespace std;

Node::Node()
    : m_prob(0.0)
    , m_size(0)
    , m_state()
    , m_children()
{
}

Node::Node(const State& state)
    : m_prob(0.0)
    , m_size(0)
    , m_state(state)
    , m_children()
{
}

Node::Node(const Board& board, int score, Player player)
    : m_prob(0.0)
    , m_size(0)
    , m_state(board, score, player)
    , m_children()
{
}

void Node::reset()
{
    m_prob = 0.0;
    m_size = 0;
    m_state.reset();
    m_children.clear();
}

void Node::reset(const Board& board, int score, Player player)
{
    m_prob = 0.0;
    m_size = 0;
    m_state.reset(board, score, player);
    m_children.clear();
}

size_t Node::expand()
{
    m_size = 0;

    switch (m_state.getPlayer()) {
    case Player::HUMAN:
        for (Direction direction : allDirections) {
            if (m_state.getBoard().isLegalMove(direction)) {
                State state_new(m_state.getBoard(), m_state.getScore(), Player::HUMAN);
                state_new.increaseScore(state_new.getBoard().performMove(direction));
                m_children.emplace_back(state_new);
                ++m_size;
            }
        }

        break;
    case Player::COMPUTER:
        for (const Board& board : m_state.getBoard().getComputerMovedBoards()) {
            for (Direction direction : allDirections) {
                if (board.isLegalMove(direction)) {
                    State state_new(board, m_state.getScore(), Player::COMPUTER);
                    state_new.increaseScore(state_new.getBoard().performMove(direction));
                    m_children.emplace_back(state_new);
                    ++m_size;
                }
            }
        }
        break;

    case Player::UNKNOWN:
    default:
        break;
    }

    return m_size;
}

} // namespace game2048