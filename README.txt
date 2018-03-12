Team ウィアブー!a simple 

Megan: Implemented the AI base and introduced the Roxanne heuristic. Attempted to build upon AI 
       improvements by overhauling the entire AI structure, including adjustable minimax depths,
       consideration of time remaining, and increased abstraction.
Evan: Created the Minimax function to depth 2 and began AI improvements by integrating the 
      Roxanne heuristic into the minimax function.

The current heuristic takes into account piecewise advantage (gain or loss in number of pieces) and positionwise advantage (via the Roxanne heuristic). The Roxanne heuristic (see below) weights different spaces on the board by the strategic advantage they provide (ex. corners are given priority since they can never be recaptured). In this way, the AI takes into account both immediate and long-term losses and gains. The roxanne heuristic was weighted so that high priority spaces gave heavy advantages, while low priotity spaces were negligible.

An attempt, contained in branch "wip", was made to improve its strategic and computational performance. These improvements include increasing the depth of the minimax search, abstracting the minimax function to allow for testing of various other strategies (other than Roxanne), accounting for time remaining and changing the strategy accordingly, and improving the board memory implementation. Due to time constraints, it was left unfinished.




Roxanne heuristic borrowed from "Analysis of Monte Carlo Technique in Othello"
by Ryan Archer.
http://citeseerx.ist.psu.edu/viewdoc/citations;jsessionid=376CC7002847ED3EB480F0029B7602D1?doi=10.1.1.468.8160

"Experiments with Monte Carlo Othello" by Hingston and Masek
http://ro.ecu.edu.au/ecuworks/4970/


