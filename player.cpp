#include "player.hpp"

///////////////////////////////////////////////////////////
// GENERAL METHODS                                       //
///////////////////////////////////////////////////////////

/**
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) : heuristic(Heuristic(&board, pattern)) {
    side = s;
    opponent = (s == WHITE) ? BLACK : WHITE;

    switch (strategy) {
        case SIMPLE:
            algorithm = simple;
            break;
        case MINIMAX:
            algorithm = minimax;
            break;
        case MC:
            algorithm = montecarlo;
            break;
        default:
            cerr << "Player error: Invalid strategy! Default to simple...\n";
            algorithm = simple;
            break;
    }
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
        } else {
            cerr << "doMove error: Opponent's move invalid!\n";
            return nullptr;
        }
    }

    // Time allocated for turn, based on number of turns and time remaining
    // Allocated time reduced by 1000ms to account for overhead
    int msTurn = (msLeft >= 0) ? msLeft / (30 - turn) - 1000 : -1;
    timer = Timer(msTurn);

    Move *move = (this->*algorithm)();
    board.doMove(move, side);
    ++turn;
    return move;
}

/**
 * Change heuristic mode
 */
void Player::setHeuristic(Pattern p) {
    pattern = p;
    heuristic = Heuristic(&board, pattern);
}

/**
 * Clean up excess allocated moves from memory
 */
void Player::deleteMoves() {
    for (auto it = allocatedMoves.begin(); it != allocatedMoves.end; ++it) {
        delete *it;
    }
    allocatedMoves.clear();
}

/**
 * Set to minimax test mode, going to depth 2 with no added heuristic
 */
void Player::testMinimax() {
    testingMinimax = true;
    depth = 2;
    setHeuristic(NONE);
    algorithm = minimax;
}

///////////////////////////////////////////////////////////
// MOVE ALGORITHMS                                       //
///////////////////////////////////////////////////////////

/**
 * Get available moves for given side
 */
std::vector<Move *> Player::getMoves(Side player) {
    std::vector<Move *> openMoves;
    Move *move;

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            move = new Move(x, y);
            if (board.checkMove(move, player)) {
                allocatedMoves.push_back(move);
                openMoves.push_back(move);
            } else {
                delete move;
            }
        }
    }

    return openMoves;
}

/**
 * Simple appoach
 * Apply only heuristic to determine next move
 */
Move *Player::simple() {
    // Naive approach -- do best move according only to heuristic
    return heuristic.getBest();
}

/**
 * Minimax approach
 * Looks to depth (specified by preset depth variable) into future,
 * picking move based on number of stones in favor of player
 */
Move *Player::minimax() {
    MoveScore best =
        recursiveMinimax((side, mmDepth - 1, std::vector<Move *>()));

    for (int i = 0; i < numPush; i++) {
        move2 = pop();
        tempBoard2 = tempBoard1->copy();  // Board at depth 2
        tempBoard2->doMove(move2, opponent);
        roxGain2 = roxGainTrans(order[to1D(move2->getX(), move2->getY())]);
        gain = tempBoard2->count(side) - tempBoard2->count(opponent);
        gain -= roxGain2;
        gain += roxGain1;
        if (gain < minGain) minGain = gain;
    }

    Move *bestMove = new Move(best<0>->getX(), best<0>->getY());
    board.doMove(bestMove, side);
    deleteMoves();
    return bestMove;
}

/**
 * Recursive helper function for minimax
 * Continue until max depth reached or time runs out
 */
MoveScore Player::recursiveMinimax(MinimaxTuple tuple) {
    MoveScore best = MoveScore(nullptr, std::numeric_limits<int>::min());

    // If not enough time left in term, abort early
    if (timer.remaining() < 1000) {
        return best;
    }

    // Unpack tuple data
    int player = tuple<0>;
    int depth = tuple<1>;
    std::vector<Move *> previous = tuple<2>;
    std::vector<double> weights = tuple<3>;

    // Get available moves given current board state
    std::vector<Move *> openMoves = getMoves(player);

    if (depth <= 1) {  // Leaf of decision tree
        Move *bestMove;
        int bestScore;

        // Find best move, given that player is supporting side
        if (player == side) {
            bestScore = std::numeric_limits<int>::min();

            for (auto m : openMoves) {
                board.doMove(m);

                double sum =
                    std::accumulate(weights.begin(), weights.end(), 0) +
                    heuristic.getWeight();
                double score = board.count(side) - board.count(opponent) + sum;

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = m;
                }

                undoMove(m);
            }
        }
        // Find worst move, given that player is opposing side
        else {
            bestScore = std::numeric_limits<int>::max();

            for (auto m : openMoves) {
                board.doMove(m);

                double sum =
                    std::accumulate(weights.begin(), weights.end(), 0) -
                    heuristic.getWeight();
                double score = board.count(side) - board.count(opponent) + sum;

                if (score < bestScore) {
                    bestScore = score;
                    bestMove = m;
                }

                undoMove(m);
            }

            bestScore *= -1;
        }

        best<0> = bestMove;
        best<1> = bestScore;
    } else {  // Node of decision tree
        for (auto m : openMoves) {
            // Update tuple data for next moves
            player = (player == side) ? opponent : side;
            --depth;
            previous.push_back(m);

            // For each possible move, try move and explore next level of tree
            board.doMove(m);

            MoveScore current =
                recursiveMinimax(MinimaxTuple(player, depth, previous));
            if (current<1>> best<1>) {
                best = current;
            }

            board.undoMove(m);
        }
    }
}

/**
 * Monte Carlo approach
 * Performs number of simulations (specified by preset simulations variable),
 * picking move from resulting most favorable outcome
 */
