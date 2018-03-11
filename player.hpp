#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <array>
#include <iostream>
#include <limits>
#include <numeric>
#include <tuple>
#include <vector>

#include "board.hpp"
#include "common.hpp"
#include "heuristic.hpp"
#include "timer.hpp"

#define PATTERN ROXANNE     // Heuristic pattern
#define STRATEGY MINIMAX    // Algorithm stategry
#define MINIMAX_DEPTH 5     // Depth for minimax algorithm
#define MC_SIMULATIONS 100  // Number of Monte Carlo simulations to run
#define WEIGHT 10

enum Strategy { SIMPLE, MINIMAX, MC };

// Tuple of (pointer to Move, int score of move)
typedef std::tuple<Move *, double> MoveScore;
// Tuple of (side, remaining depth, heuristic weight)
typedef std::tuple<Side, int, double> MinimaxTuple;

class Player {
   private:
    Board board;
    Timer timer;
    int turn = 0;
    Side side;
    Side opponent;
    Pattern pattern = PATTERN;
    Strategy strategy = STRATEGY;
    Heuristic heuristic;
    Move *(Player::*algorithm)();  // Pointer to move algorithm

    // Algorithm specific variables and methods
    std::vector<Move *> getMoves(Side player);
    int mmDepth = MINIMAX_DEPTH;
    int mcSimulations = MC_SIMULATIONS;

    // Different move algorithms
    Move *simple();   // Apply just heuristic
    Move *minimax();  // Apply minimax (with heuristic)
    MoveScore recursiveMinimax(MinimaxTuple tuple);
    Move *montecarlo();  // Apply Monte Carlo (with heuristic)

    // Track and manage memory of moves
    std::vector<Move *> allocatedMoves;
    void deleteMoves();

   public:
    Player(Side s);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    void setBoard(Board *b) { board = *(b->copy()); }
    void setHeuristic(Pattern p);

    // Set to minimax testing mode
    bool testingMinimax = false;
    void testMinimax();
};

#endif
