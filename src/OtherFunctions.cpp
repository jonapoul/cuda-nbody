#include <string>
using namespace std;

#include "Config.h"
#include "OtherFunctions.h"
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

}
