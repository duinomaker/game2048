#ifndef __GAME2048_STATE__
#define __GAME2048_STATE__
#include "Board.hpp"
#include "Move.hpp"
#include "Player.hpp"
#include <cstdint>
#include <iostream>
#include <list>

namespace game2048 {

class State {
    friend std::ostream& operator<<(std::ostream& out, const State& state);

private:
    int m_score;
    int m_score_delta;
    Player m_player;
    Move m_last_move;
    std::uint64_t m_board;

public:
    State(
        std::uint64_t board = 0,
        Player player = Player::UNKNOWN, Move last_move = Move::UNKNOWN,
        int score = 0, int score_delta = 0);

    int getScore() const { return m_score; }

    void setScore(int score) { m_score = score; }

    Player getPlayer() const { return m_player; }

    Move getLastMove() const { return m_last_move; }

    std::list<State> getAllSubStates() const;

    bool isEndOfGame() const;
};

} // namespace game2048

#endif // __GAME2048_STATE__