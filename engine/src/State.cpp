#include "State.hpp"
#include "Board.hpp"
#include "Direction.hpp"
#include "Player.hpp"

namespace game2048 {

State::State()
    : m_score(0)
    , m_score_delta(0)
    , m_player(Player::UNKNOWN)
    , m_board()
{
}

State::State(const State& state)
    : m_score(state.m_score)
    , m_score_delta(state.m_score_delta)
    , m_player(state.m_player)
    , m_board(state.m_board)
{
}

State::State(const Board& board, int score, Player player)
    : m_score(score)
    , m_score_delta(0)
    , m_player(player)
    , m_board(board)
{
}

void State::reset()
{
    m_score = 0;
    m_score_delta = 0;
    m_player = Player::UNKNOWN;
    m_board.reset();
}

void State::reset(const State& state)
{
    m_score = state.m_score;
    m_score_delta = state.m_score_delta;
    m_player = state.m_player;
    m_board.reset(state.m_board);
}

void State::reset(const Board& board, int score, Player player)
{
    m_score = score;
    m_score_delta = 0;
    m_player = player;
    m_board.reset(board);
}

void State::increaseScore(int score_delta)
{
    m_score += m_score_delta = score_delta;
}

bool State::isEndOfGame() const
{
    for (Direction direction : allDirections) {
        if (m_board.isLegalMove(direction))
            return false;
    }
    return true;
}

} // namespace game2048