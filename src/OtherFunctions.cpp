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

}
