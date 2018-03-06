#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <array>
#include <iostream>
#include <vector>
#include "board.hpp"
#include "common.hpp"
using namespace std;

#define INF 1000000000
#define STRATEGY COMBINATION
#define WEIGHT_C 10

class Player {
   private:
    enum Strategy { ROXANNE, MINIMAX, COMBINATION };
    Board board;
    Side side;
    Side opponent;
    int order[64];                // For Roxanne, priority order of squares
    vector<array<int, 2>> queue;  // For Roxanne, {ind, priority} of squares
    vector<Move *> stack;         // For Minimaz, Combination
    Strategy strat;
    void setOrder();
    void setSquares(vector<array<int, 2>> squares, int priority);
    void loadQueue();
    void dequeueMove(Move *move);
    int loadStack(Side s, Board *b);
    Move *pop();
    int to1D(int x, int y) { return x + 8 * y; }
    int toX(int ind) { return ind % 8; }
    int toY(int ind) { return ind / 8; }
    double roxGainTrans(double roxGain);
    void printStack();

   public:
    Player(Side s);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    void setBoard(Board *b);
};

#endif
