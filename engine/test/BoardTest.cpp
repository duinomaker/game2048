#include "Board.hpp"
#include "Move.hpp"
#include "SimpleTest.hpp"
#include <cstdint>
#include <iostream>
#include <list>

using namespace std;
using namespace game2048;

INIT_TEST(BoardTest)

TEST(constructor_default)
{
    uint64_t board = 0;
    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], 0);
}
ENDTEST()

TEST(constructor_and_toarray)
{
    int arr_raw[4][4] = {
        { 1, 1, 2, 3 },
        { 2, 5, 4, 7 },
        { 5, 3, 5, 3 },
        { 9, 9, 8, 1 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_raw[i][j]);
}
ENDTEST()

TEST(performMove_up_1)
{
    int arr_raw[4][4] = {
        { 1, 3, 4, 0 },
        { 1, 3, 4, 0 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    int arr_expected[4][4] = {
        { 2, 4, 5, 5 },
        { 0, 3, 5, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::UP), 84);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_up_2)
{
    int arr_raw[4][4] = {
        { 3, 0, 3, 1 },
        { 0, 7, 5, 7 },
        { 6, 2, 2, 2 },
        { 3, 4, 4, 5 }
    };
    int arr_expected[4][4] = {
        { 3, 7, 3, 1 },
        { 6, 2, 5, 7 },
        { 3, 4, 2, 2 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::UP), 0);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_up_3)
{
    int arr_raw[4][4] = {
        { 0, 0, 0, 0 },
        { 1, 7, 5, 7 },
        { 6, 0, 4, 9 },
        { 0, 0, 4, 0 }
    };
    int arr_expected[4][4] = {
        { 1, 7, 5, 7 },
        { 6, 0, 5, 9 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::UP), 32);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_down_1)
{
    int arr_raw[4][4] = {
        { 1, 3, 4, 0 },
        { 1, 3, 4, 0 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    int arr_expected[4][4] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 3, 5, 0 },
        { 2, 4, 5, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::DOWN), 84);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_down_2)
{
    int arr_raw[4][4] = {
        { 3, 7, 5, 1 },
        { 0, 7, 2, 6 },
        { 6, 2, 0, 6 },
        { 3, 4, 4, 5 }
    };
    int arr_expected[4][4] = {
        { 0, 0, 0, 0 },
        { 3, 8, 5, 1 },
        { 6, 2, 2, 7 },
        { 3, 4, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::DOWN), 384);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_down_3)
{
    int arr_raw[4][4] = {
        { 0, 0, 0, 0 },
        { 1, 7, 5, 7 },
        { 6, 0, 4, 9 },
        { 0, 0, 4, 0 }
    };
    int arr_expected[4][4] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 1, 0, 5, 7 },
        { 6, 7, 5, 9 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::DOWN), 32);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_left_1)
{
    int arr_raw[4][4] = {
        { 1, 1, 0, 0 },
        { 3, 3, 3, 0 },
        { 4, 4, 4, 4 },
        { 0, 0, 0, 5 }
    };
    int arr_expected[4][4] = {
        { 2, 0, 0, 0 },
        { 4, 3, 0, 0 },
        { 5, 5, 0, 0 },
        { 5, 0, 0, 0 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::LEFT), 84);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_left_2)
{
    int arr_raw[4][4] = {
        { 3, 0, 3, 1 },
        { 0, 7, 5, 7 },
        { 6, 2, 2, 2 },
        { 3, 4, 4, 5 }
    };
    int arr_expected[4][4] = {
        { 4, 1, 0, 0 },
        { 7, 5, 7, 0 },
        { 6, 3, 2, 0 },
        { 3, 5, 5, 0 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::LEFT), 56);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_left_3)
{
    int arr_raw[4][4] = {
        { 0, 0, 0, 0 },
        { 1, 7, 5, 7 },
        { 6, 0, 0, 9 },
        { 0, 0, 4, 0 }
    };
    int arr_expected[4][4] = {
        { 0, 0, 0, 0 },
        { 1, 7, 5, 7 },
        { 6, 9, 0, 0 },
        { 4, 0, 0, 0 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::LEFT), 0);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_right_1)
{
    int arr_raw[4][4] = {
        { 1, 1, 0, 0 },
        { 3, 3, 3, 0 },
        { 4, 4, 4, 4 },
        { 0, 0, 0, 5 }
    };
    int arr_expected[4][4] = {
        { 0, 0, 0, 2 },
        { 0, 0, 3, 4 },
        { 0, 0, 5, 5 },
        { 0, 0, 0, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::RIGHT), 84);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_right_2)
{
    int arr_raw[4][4] = {
        { 3, 0, 3, 1 },
        { 0, 7, 5, 7 },
        { 6, 2, 2, 2 },
        { 3, 4, 4, 5 }
    };
    int arr_expected[4][4] = {
        { 0, 0, 4, 1 },
        { 0, 7, 5, 7 },
        { 0, 6, 2, 3 },
        { 0, 3, 5, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::RIGHT), 56);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(performMove_right_3)
{
    int arr_raw[4][4] = {
        { 0, 0, 0, 0 },
        { 1, 7, 5, 7 },
        { 6, 0, 0, 9 },
        { 0, 0, 4, 0 }
    };
    int arr_expected[4][4] = {
        { 0, 0, 0, 0 },
        { 1, 7, 5, 7 },
        { 0, 0, 6, 9 },
        { 0, 0, 0, 4 }
    };
    uint64_t board = Board::fromArray(arr_raw);
    EXPECT_EQ(Board::performMove(board, Move::RIGHT), 0);

    int arr[4][4];
    Board::toArray(board, arr);

    for (int i = 0; i != 4; ++i)
        for (int j = 0; j != 4; ++j)
            EXPECT_EQ(arr[i][j], arr_expected[i][j]);
}
ENDTEST()

TEST(isLegalMove_up_1)
{
    int arr_raw[4][4] = {
        { 1, 3, 4, 0 },
        { 1, 3, 2, 0 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::UP))
    Board::performMove(board, Move::UP);
    EXPECT_FALSE(Board::isLegalMove(board, Move::UP))
}
ENDTEST()

TEST(isLegalMove_up_2)
{
    int arr_raw[4][4] = {
        { 4, 4, 4, 3 },
        { 1, 3, 4, 2 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::UP))
    Board::performMove(board, Move::UP);
    EXPECT_TRUE(Board::isLegalMove(board, Move::UP))
    Board::performMove(board, Move::UP);
    EXPECT_FALSE(Board::isLegalMove(board, Move::UP))
}
ENDTEST()

TEST(isLegalMove_down_1)
{
    int arr_raw[4][4] = {
        { 1, 3, 4, 0 },
        { 1, 3, 2, 0 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::DOWN))
    Board::performMove(board, Move::DOWN);
    EXPECT_FALSE(Board::isLegalMove(board, Move::DOWN))
}
ENDTEST()

TEST(isLegalMove_down_2)
{
    int arr_raw[4][4] = {
        { 4, 4, 4, 3 },
        { 1, 3, 4, 2 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::DOWN))
    Board::performMove(board, Move::DOWN);
    EXPECT_TRUE(Board::isLegalMove(board, Move::DOWN))
    Board::performMove(board, Move::DOWN);
    EXPECT_FALSE(Board::isLegalMove(board, Move::DOWN))
}
ENDTEST()

TEST(isLegalMove_left_1)
{
    int arr_raw[4][4] = {
        { 1, 3, 4, 0 },
        { 1, 3, 4, 0 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::LEFT))
    Board::performMove(board, Move::LEFT);
    EXPECT_FALSE(Board::isLegalMove(board, Move::LEFT))
}
ENDTEST()

TEST(isLegalMove_left_2)
{
    int arr_raw[4][4] = {
        { 4, 4, 4, 3 },
        { 1, 3, 2, 2 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::LEFT))
    Board::performMove(board, Move::LEFT);
    EXPECT_TRUE(Board::isLegalMove(board, Move::LEFT))
    Board::performMove(board, Move::LEFT);
    EXPECT_FALSE(Board::isLegalMove(board, Move::LEFT))
}
ENDTEST()

TEST(isLegalMove_right_1)
{
    int arr_raw[4][4] = {
        { 1, 3, 4, 0 },
        { 1, 3, 4, 0 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::RIGHT))
    Board::performMove(board, Move::RIGHT);
    EXPECT_FALSE(Board::isLegalMove(board, Move::RIGHT))
}
ENDTEST()

TEST(isLegalMove_right_2)
{
    int arr_raw[4][4] = {
        { 4, 4, 4, 3 },
        { 1, 3, 2, 2 },
        { 0, 3, 4, 0 },
        { 0, 0, 4, 5 }
    };
    uint64_t board = Board::fromArray(arr_raw);

    EXPECT_TRUE(Board::isLegalMove(board, Move::RIGHT))
    Board::performMove(board, Move::RIGHT);
    EXPECT_TRUE(Board::isLegalMove(board, Move::RIGHT))
    Board::performMove(board, Move::RIGHT);
    EXPECT_FALSE(Board::isLegalMove(board, Move::RIGHT))
}
ENDTEST()

TERMINATE_TEST()