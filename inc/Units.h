#ifndef CUDANBODY_UNITS_H
#define CUDANBODY_UNITS_H

#include <string>
#include "Config.h"

namespace cnb {

class Units {
public:
   Units(std::string const& filename);

   struct Unit {
      cnb_float val;
      std::string name;
   };

   Unit time;
   Unit distance;
   Unit mass;
};

} /* namespace cnb */

#endif