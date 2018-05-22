#ifndef CUDANBODY_CONSTANTS_H
#define CUDANBODY_CONSTANTS_H

#include <vector>
#include "Config.h"
#include "Units.h"

namespace cnb {

class Constants {
public:
   Constants(std::string const& filename,
             Units const& units);

   double c;         /* speed of light */
   double e;         /* electron charge */
   double G;         /* graviational constant */
   double h;         /* planck */
   double hbar;      /* reduced planck */
   double k_b;       /* boltzmann */
   double m_e;       /* electron mass */
   double m_p;       /* proton mass */
   double epsilon_0; /* permittivity of free space */
   double mu_0;      /* permeability of free space */

private:
   void ReadFromFile(std::string const& filename);
   void ConvertUnits(Units const& units);
   void Print() const;
};


} /* namespace cnb */

#endif