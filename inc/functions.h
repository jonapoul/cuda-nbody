#ifndef CUDANBODY_FUNCTIONS_H
#define CUDANBODY_FUNCTIONS_H

#include <string>
#include "Config.h"

namespace cnb {

float rsqrt(float number);

void check_range_impl(cnb_float const x,
                      cnb_float const min,
                      cnb_float const max,
                      char const * const str);

typedef enum { ALIGN_LEFT, ALIGN_MID, ALIGN_RIGHT } TextAlignment;
std::string padded(std::string const& input,
                   size_t const size,
                   TextAlignment const align = ALIGN_LEFT);

std::string timestamp();

} /* namespace cnb */

#endif