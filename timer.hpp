#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <limits>

/**
 * Basic timer that tracks elapsed and remaining time
 */
class Timer {
   private:
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
    int msStart;

   public:
    Timer() { Timer(-1); }
    Timer(int msLeft) {
        start = std::chrono::system_clock::now();
        msStart = msLeft;
    }

    int initial() { return msStart; }
    int elapsed() {
        end = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start)
            .count();
    }
    int remaining() {
        return (msStart < 0) ? std::numeric_limits<int>::max()
                             : msStart - elapsed();
    }
};

#endif  // __TIMER_H__