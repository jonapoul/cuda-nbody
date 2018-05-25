#ifndef CUDANBODY_TIMER_H
#define CUDANBODY_TIMER_H

#include <chrono>
#include "Config.h"

namespace cnb {

class Timer {
public:
   Timer();
   void reset();
   cnb_float elapsed();

private:
   typedef std::chrono::high_resolution_clock clock;
   std::chrono::time_point<clock> start;
   std::chrono::time_point<clock> prev;
};

} /* namespace cnb */

#endif
