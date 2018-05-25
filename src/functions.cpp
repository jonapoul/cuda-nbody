#include <ctime>
#include <string>
using namespace std;

#include "Config.h"
#include "functions.h"
#include "TeeStream.h"

namespace cnb {

float rsqrt(float number) {
   long i;
   float x2, y;
   const float threehalfs = 1.5F;

   x2 = number * 0.5F;
   y  = number;
   i  = * ( long * ) &y;                       // evil floating point bit level hacking
   i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
   y  = * ( float * ) &i;
   y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
   return y;
}

void check_range_impl(cnb_float const x,
                      cnb_float const min,
                      cnb_float const max,
                      char const * const str) {
   if (x < min || x > max) {
      terr << "'" << str << "' is out of range\n";
      exit(1);
   }
}

string padded(string const& input,
              size_t const size,
              TextAlignment const align) {
   if (size <= input.length()) {
      return input;
   }
   string const gap(size - input.length(), ' ');
   switch (align) {
      case ALIGN_LEFT:  return input + gap;
      case ALIGN_MID:   return string(size/2,' ') + input + string(size-size/2, ' ');
      case ALIGN_RIGHT: return gap + input;
   }
   terr << "Unknown text alignment\n";
   exit(1);
   return "";
}

string timestamp() {
   time_t rawtime;
   struct tm * timeinfo;
   char buffer[32];
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);
   return string(buffer);
}

string secsToString(cnb_float const secs) {
   char buf[32];
   int const hr  = secs / 3600;
   int const min = int(secs / 60) % 60;
   cnb_float sec = secs;
   while (sec > 60) sec -= 60;
   snprintf(buf, 32, "%02dh %02dm %02.0fs", hr, min, sec);
   return buf;
}

}
