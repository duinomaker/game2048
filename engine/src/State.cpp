#include "State.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include "Player.hpp"
#include <cstdint>
#include <list>
#include <utility>

namespace game2048 {

using namespace std;

State::State(board_t board, Player player, Move last_move, int score, int score_delta)
    : m_score(score)
    , m_score_delta(score_delta)
    , m_player(player)
    , m_last_move(last_move)
    , m_board(board)
{
}

list<State> State::getAllSubStates() const
{
    list<State> states;
    switch (m_player) {
    case Player::HUMAN:
        for (Move direction : allDirections) {
            if (Board::isLegalMove(m_board, direction)) {
                board_t board_new = m_board;
                int score_delta = Board::performMove(board_new, direction);
                states.emplace_back(board_new,
                    Player::COMPUTER, direction,
                    m_score + score_delta, score_delta);
            }
        }
        break;
    case Player::COMPUTER:
        for (Move val : allComputerMoves) {
            for (const pair<int, int>& coord : Board::getAllSlots(m_board)) {
                board_t board_new = m_board;
                Board::performMove(board_new, val, coord);
                states.emplace_back(board_new,
                    Player::HUMAN, val,
                    m_score, 0);
            }
        }
        break;

    case Player::UNKNOWN:
    default:
        break;
    }
    return states;
}

bool State::isEndOfGame() const
{
    for (Move direction : allDirections)
        if (Board::isLegalMove(m_board, direction))
            return false;
    return true;
}

ostream& operator<<(ostream& out, const State& state)
{
    out << "=========================\n";
    Board::represent(state.m_board, out);
    out << "\nLast Move   : " << state.m_last_move
        << "\nNext Player : " << state.m_player
        << "\nScore       : " << state.m_score << " (+" << state.m_score_delta << ")";
    return out;
}

} // namespace game2048