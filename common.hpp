#ifndef __COMMON_H__
#define __COMMON_H__

enum Side { WHITE, BLACK };

// Helper functions to translate coordinates
int to1D(int x, int y) { return x + 8 * y; }
int toX(int ind) { return ind % 8; }
int toY(int ind) { return ind / 8; }

class Move {
   public:
    int x, y;
    Move(int x, int y) {
        this->x = x;
        this->y = y;
    }
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

#endif
