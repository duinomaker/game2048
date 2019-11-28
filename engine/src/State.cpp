#include "State.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include "Player.hpp"
#include <cstdint>
#include <list>

namespace game2048 {

using namespace std;

State::State(board_t board, Player player, Move last_move)
    : m_player(player)
    , m_last_move(last_move)
    , m_board(board)
{
}

list<State> State::getAllSubStates() const
{
    list<State> states;
    if (m_player == Player::HUMAN) {
        for (Move direction : allDirections) {
            if (Board::isLegalMove(m_board, direction)) {
                board_t board_new = m_board;
                Board::performMove(board_new, direction);
                states.emplace_back(board_new, Player::COMPUTER, direction);
            }
        }
    } else {
        board_t temp = m_board;
        for (board_t val = 1; val; val <<= 4) {
            if (!(temp & UINT64_C(0xF))) {
                states.emplace_back(m_board | val, Player::HUMAN, Move::RANDOM_1);
                states.emplace_back(m_board | (val << 1), Player::HUMAN, Move::RANDOM_2);
            }
            temp >>= 4;
        }
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
        << "\nNext Player : " << state.m_player;
    return out;
}

} // namespace game2048