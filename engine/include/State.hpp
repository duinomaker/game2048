#ifndef __GAME2048_STATE__
#define __GAME2048_STATE__
#include "Board.hpp"
#include "Direction.hpp"
#include "Player.hpp"

namespace game2048 {

class State {
private:
    int m_score;
    int m_score_delta;
    Player m_player;
    Board m_board;

public:
    State();

    State(const State& state);

    State(const Board& board, int score, Player player);

    void reset();

    void reset(const State& state);

    void reset(const Board& board, int score, Player player);

    Board& getBoard() { return m_board; }

    const Board& getBoard() const { return m_board; }

    const int getScore() const { return m_score; }

    void setScore(int score) { m_score = score; }

    void increaseScore(int score_delta);

    const Player getPlayer() const { return m_player; }

    bool isEndOfGame() const;
};

} // namespace game2048

#endif // __GAME2048_STATE__