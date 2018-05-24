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

double Timer::elapsed() {
   double dt = duration_cast<milliseconds>(clock::now() - prev).count() / 1e3;
   prev = clock::now();
   return dt;
}
