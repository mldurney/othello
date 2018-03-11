#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include <array>
#include <vector>
#include "board.hpp"
#include "common.hpp"
using namespace std;

enum Pattern { NONE, ROXANNE };

class Heuristic {
   private:
    Board* board;
    Pattern pattern;
    void update();

   public:
    Heuristic(Board* b, Pattern p);
    ~Heuristic() {}

    // Heuristic methods
    Move* getBest();
    double getWeight(int ind);
    double getWeight(int x, int y) { return getWeight(to1D(x, y)); }
};

#endif  // __HEURISTIC_H__