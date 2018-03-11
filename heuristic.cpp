#include "heuristic.hpp"

Heuristic::Heuristic(Board* b, Pattern p) : board(b), pattern(p) {
    // TODO
}

void Heuristic::update() {
    // TODO
}

Move* Heuristic::getBest() {
    // TODO
    update();
    return nullptr;
}

double Heuristic::getWeight(int ind) {
    // TODO
    return -1;
}