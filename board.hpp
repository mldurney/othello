#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>

#include "common.hpp"

class Board {
   private:
    std::bitset<64> black;
    std::bitset<64> taken;

    bool occupied(int ind);
    bool get(Side side, int ind);
    void set(Side side, int ind);
    void unset(int ind);
    bool onBoard(int ind);
    bool occupied(int x, int y) { return occupied(to1D(x, y)); }
    bool get(Side side, int x, int y) { return get(side, to1D(x, y)); }
    void set(Side side, int x, int y) { set(side, to1D(x, y)); }
    void unset(int x, int y) { unset(to1D(x, y)); }
    bool onBoard(int x, int y);

   public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool isOccupied(int ind) { return occupied(ind); }
    bool isOccupied(int x, int y) { return occupied(x, y); }
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    bool undoMove(Move *m);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
};

#endif
