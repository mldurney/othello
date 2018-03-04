#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <array>
#include <iostream>
#include <vector>
#include "board.hpp"
#include "common.hpp"
using namespace std;

class Player {
   private:
    Board board;
    Side side;
    Side opponent;
    int order[64];                // For Roxanne, priority order of squares
    vector<array<int, 2>> queue;  // For Roxanne, {ind, priority} of squares

    void setOrder();
    void setSquares(vector<array<int, 2>> squares, int priority);
    void loadQueue();
    int to1D(int x, int y) { return x + 8 * y; }
    int toX(int ind) { return ind / 8; }
    int toY(int ind) { return ind % 8; }

   public:
    Player(Side s);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
