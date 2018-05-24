#ifndef CUDANBODY_TIMER_H
#define CUDANBODY_TIMER_H

#include <chrono>

namespace cnb {

class Timer {
public:
   Timer();
   void reset();
   double elapsed();

private:
   typedef std::chrono::high_resolution_clock clock;
   std::chrono::time_point<clock> start;
   std::chrono::time_point<clock> prev;
};

} /* namespace cnb */

#endif
