#include "player.hpp"

/**
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) {
    strat = STRATEGY;
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    if (testingMinimax)
        strat = MINIMAX;

    side = s;
    opponent = (s == WHITE) ? BLACK : WHITE;
    setOrder();
    if (strat == ROXANNE)
        loadQueue();
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
    // Process opponent's move
    if (opponentsMove != nullptr) {
        if (board.checkMove(opponentsMove, opponent)) {
            board.doMove(opponentsMove, opponent);
            dequeueMove(opponentsMove);
        } else {
            cerr << "doMove error: Opponent's move invalid!\n";
            return nullptr;
        }
    }

    if (strat == MINIMAX) {
        // Minimax approach. Looks a depth of 2 into the future and picks a
        // move based on the number of stones
        Move *bestMove = nullptr, *move1, *move2;
        int maxMinGain = -(INF), minGain;
        int numPush;
        int gain;
        Board *tempBoard1, *tempBoard2;

        // Examines all possible moves for player
        loadStack(side, &board);
        while (stack.size() > 0) {
            minGain = INF;
            move1 = pop();
            tempBoard1 = board.copy();            // Board at depth 1
            tempBoard1->doMove(move1, side);

            // For each player move, examines possible opponent moves
            numPush = loadStack(opponent, tempBoard1);
            for (int i = 0; i < numPush; i++) {
                move2 = pop();
                tempBoard2 = tempBoard1->copy();   // Board at depth 2
                tempBoard2->doMove(move2, opponent);
                gain = tempBoard2->count(side) - tempBoard2->count(opponent);
                if (gain < minGain) 
                    minGain = gain;
            }

            // Picks player's best move
            if (minGain > maxMinGain) {
                bestMove = move1;
                maxMinGain = minGain;
            }
        }

        board.doMove(bestMove, side);
        return bestMove;

    } else if (strat == COMBINATION) {
        // Combination approach. Looks a depth of 2 into the future and picks a
        // move based on the number of stones and roxanne priotity
        Move *bestMove = nullptr, *move1, *move2;
        double maxMinGain = -(INF), minGain;
        double roxGain1, roxGain2;
        int numPush;
        double gain;
        Board *tempBoard1, *tempBoard2;

        // Examines all possible moves for player
        loadStack(side, &board);
        while (stack.size() > 0) {
            minGain = INF;
            move1 = pop();
            tempBoard1 = board.copy();            // Board at depth 1
            tempBoard1->doMove(move1, side);
            roxGain1 = roxGainTrans(order[to1D(move1->getX(), move1->getY())]);
            
            // For each player move, examines possible opponent moves
            numPush = loadStack(opponent, tempBoard1);
            for (int i = 0; i < numPush; i++) {
                move2 = pop();
                tempBoard2 = tempBoard1->copy();   // Board at depth 2
                tempBoard2->doMove(move2, opponent);
                roxGain2 = roxGainTrans(order[to1D(move2->getX(), move2->getY())]);
                gain = tempBoard2->count(side) - tempBoard2->count(opponent);
                gain -= roxGain2;
                gain += roxGain1;
                if (gain < minGain) 
                    minGain = gain;
            }

            // Picks player's best move
            if (minGain > maxMinGain) {
                bestMove = move1;
                maxMinGain = minGain;
            }
        }

        board.doMove(bestMove, side);
        return bestMove;

    } else if (strat == ROXANNE) {
        // Naive approach -- do first possible move in Roxanne queue
        Move *move = new Move(0, 0);
        for (vector<array<int, 2>>::iterator it = queue.begin(); it != queue.end();
             ++it) {
            move->setX(toX((*it)[0]));
            move->setY(toY((*it)[0]));

            if (board.checkMove(move, side)) {
                queue.erase(it);
                board.doMove(move, side);
                return move;
            }
        }
    }


    // No open moves, so pass
    return nullptr;
}

double Player::roxGainTrans(double roxGain) {
    return WEIGHT_C/(roxGain * roxGain * roxGain);
}

/**
 * For minimax, pushes stack of valid moves, in no particular oder
 * @return number of moves pushed
 */
int Player::loadStack(Side s, Board *b) {
    Move *move;
    int num = 0;
    for (int i = 0; i < 64; ++i) {
        move = new Move(toX(i), toY(i));
        if (b->checkMove(move,s)) {
            num++;
            stack.insert(stack.begin(), move);
        }
    }
    return num;
}

/**
 * Pops a move from the stack
 */
 Move *Player::pop() {
    Move *move = stack[0];
    stack.erase(stack.begin());
    return move;
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
    vector<array<int, 2>>::iterator it;

    for (int i = 0; i < 64; ++i) {
        if (board.isOccupied(toX(i), toY(i))) {
            continue;
        }

        for (it = queue.begin(); it != queue.end(); ++it) {
            // Insert index, priority pair as early as possible in queue
            if (order[i] <= (*it)[1]) {
                queue.insert(it, {i, order[i]});
                break;
            }
        }

        // If index not inserted earlier, then priority greater yet
        if (it == queue.end()) {
            queue.push_back({i, order[i]});
        }
    }
}

/**
 * Remove move from queue
 */
void Player::dequeueMove(Move *move) {
    int ind = to1D(move->getX(), move->getY());

    for (vector<array<int, 2>>::iterator it = queue.begin(); it != queue.end();
         ++it) {
        if ((*it)[0] == ind) {
            queue.erase(it);
            return;
        }
    }
}

// Sets the board to the given board
void Player::setBoard(Board *b) {
    board = *(b->copy());
}

// Prints the stack for testing purposes
void Player::printStack() {
    for (Move *m: stack) {
        cerr << "(" << m->getX() << ", " << m->getY() << ") ";
    }
    cerr << endl;
}