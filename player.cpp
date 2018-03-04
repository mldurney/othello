#include "player.hpp"

/**
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) {
    cerr << "start init player...\n";

    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    side = s;
    opponent = (s == WHITE) ? BLACK : WHITE;
    setOrder();
    loadQueue();

    cerr << "done init player...\n";
}

/**
 * Destructor for the player.
 */
Player::~Player() {}

/**
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    cerr << "start opponent check...\n";
    // Process opponent's move
    if (opponentsMove != nullptr) {
        if (board.checkMove(opponentsMove, opponent)) {
            board.doMove(opponentsMove, opponent);
        } else {
            delete opponentsMove;
            cerr << "doMove error: Opponent's move invalid!\n";
            return nullptr;
        }

        delete opponentsMove;
    }
    cerr << "done opponent check...\n";

    cerr << "start make move...\n";
    // Naive approach -- do first possible move in Roxanne queue
    Move *move = new Move(0, 0);
    for (vector<array<int, 2>>::iterator it = queue.begin(); it != queue.end();
         ++it) {
        cerr << "start check move...\n";
        move->setX(toX((*it)[0]));
        move->setX(toY((*it)[1]));

        if (board.checkMove(move, side)) {
            queue.erase(it);
            board.doMove(move, side);
            return move;
        }
        cerr << "done make move...\n";
    }
    cerr << "done make make..\n";

    return nullptr;
}

/**
 * Set move priority order according to Roxanne algorithm
 */
void Player::setOrder() {
    // Set corners to priority 1
    vector<array<int, 2>> squares;
    squares.insert(squares.end(), {{0, 0}});
    setSquares(squares, 1);

    // Set center 4x4 to priority 2
    squares.clear();
    squares.insert(squares.end(), {{2, 2}, {2, 3}, {3, 2}});
    setSquares(squares, 2);

    // Set edges, not adjacent to corner, to priority 3
    squares.clear();
    squares.insert(squares.end(), {{0, 2}, {0, 3}, {2, 0}, {3, 0}});
    setSquares(squares, 3);

    // Set inner-edges, not diagonal to corner, to priority 4
    squares.clear();
    squares.insert(squares.end(), {{1, 2}, {1, 3}, {2, 1}, {3, 1}});
    setSquares(squares, 4);

    // Set squares surrounding corder to priority 5
    squares.clear();
    squares.insert(squares.end(), {{0, 1}, {1, 0}, {1, 1}});
    setSquares(squares, 5);
}

/**
 * Set move priority for squares and rotationally symmetrical counterparts
 * Uses x, y coordinates for square in upper-left quadrant
 */
void Player::setSquares(vector<array<int, 2>> squares, int priority) {
    for (auto &s : squares) {
        if (s[0] < 0 || s[0] >= 4 || s[1] < 0 || s[1] >= 4) {
            cerr << "setSquares error: Invalid index! Skipping square...\n";
            continue;
        }

        int ul = to1D(s[0], s[1]);          // Upper-left square
        int ur = to1D(7 - s[0], s[1]);      // Upper-right square
        int ll = to1D(s[0], 7 - s[1]);      // Lower-left square
        int lr = to1D(7 - s[0], 7 - s[1]);  // Lower-right square

        order[ul] = order[ur] = order[ll] = order[lr] = priority;
    }
}

/**
 * Load queue of moves to attempt first, based on priority order
 */
void Player::loadQueue() {
    for (int i = 0; i < 64; ++i) {
        if (board.isOccupied(toX(i), toY(i))) {
            continue;
        }

        for (vector<array<int, 2>>::iterator it = queue.begin();
             it != queue.end(); ++it) {
            // Insert index, priority pair as early as possible in queue
            if (order[i] <= (*it)[1]) {
                queue.insert(it, {i, order[i]});
            }

            // If index not inserted earlier, then priority greater yet
            queue.push_back({i, order[i]});
        }
    }
}