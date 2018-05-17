#ifndef CUDANBODY_CONSTANTS_H
#define CUDANBODY_CONSTANTS_H

#include "Config.h"

class Constants {
public:
   Constants(std::string const& filename);

   cnb_float G;   /* gravitational constant (kg-1 m3 s-2) */
   cnb_float M_E; /* earth mass (kg) */
   cnb_float AU;
};

#endif