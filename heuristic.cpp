#include "heuristic.hpp"

Heuristic::Heuristic(Board* b, Pattern p) : board(b), pattern(p) {
    // TODO
}

void Heuristic::update() {
    // TODO
}

Move* Heuristic::getBest(Side side) {
    // TODO
    update();
    return nullptr;
}

double Heuristic::getWeight(Side side, int ind) {
    // TODO
    return -1;
}