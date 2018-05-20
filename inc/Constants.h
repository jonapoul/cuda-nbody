#ifndef CUDANBODY_CONSTANTS_H
#define CUDANBODY_CONSTANTS_H

#include "Config.h"

namespace cnb {

class Constants {
public:
   Constants(std::string const& filename);

   cnb_float c;         /* speed of light */
   cnb_float e;         /* electron charge */
   cnb_float G;         /* graviational constant */
   cnb_float h;         /* planck */
   cnb_float hbar;      /* reduced planck */
   cnb_float H_0;        /* hubble */
   cnb_float k_b;       /* boltzmann */
   cnb_float m_e;       /* electron mass */
   cnb_float m_p;       /* proton mass */
   cnb_float epsilon_0; /* permittivity of free space */
   cnb_float mu_0;      /* permeability of free space */
   cnb_float sigma;     /* stefan boltzmann */
};

} /* namespace cnb */

#endif