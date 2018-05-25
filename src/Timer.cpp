#include <chrono>
#include <string>
using namespace std::chrono;

#include "Timer.h"
#include "TeeStream.h"
using namespace cnb;

Timer::Timer() 
      : start(clock::now()), prev(clock::now()) { }

void Timer::reset() {
   start = clock::now();
   prev  = clock::now();
}

cnb_float Timer::elapsed() {
   cnb_float dt = duration_cast<microseconds>(clock::now() - prev).count() / 1e6;
   return dt;
}
