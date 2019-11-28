#ifndef __GAME2048_STATE__
#define __GAME2048_STATE__
#include "Board.hpp"
#include "Move.hpp"
#include "Player.hpp"
#include <iostream>
#include <list>

namespace game2048 {

class State {
    friend std::ostream& operator<<(std::ostream& out, const State& state);

private:
    Player m_player;
    Move m_last_move;
    board_t m_board;

public:
    State(board_t board = 0, Player player = Player::UNKNOWN, Move last_move = Move::UNKNOWN);

    Player getPlayer() const { return m_player; }

    Move getLastMove() const { return m_last_move; }

    board_t getBoard() const { return m_board; }

    std::list<State> getAllSubStates() const;

    bool isEndOfGame() const;
};

} // namespace game2048

#endif // __GAME2048_STATE__