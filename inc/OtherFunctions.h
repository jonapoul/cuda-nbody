#include "Config.h"

namespace cnb {

float rsqrt(float number);
void check_range_impl(cnb_float const x,
                      cnb_float const min,
                      cnb_float const max,
                      char const * const str);

} /* namespace cnb */
