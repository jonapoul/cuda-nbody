#ifndef CUDANBODY_UNITS_H
#define CUDANBODY_UNITS_H

#include <string>
#include <vector>
#include "Config.h"

namespace cnb {

struct UnitEntry {
   std::string name = "unknown";
   cnb_float val    = -1; // in terms of the base SI unit. e.g. fm = 1e-15 m, hr = 3600 s
};

typedef enum {
   TIME,
   LENGTH,
   MASS,
   CHARGE,
   NumUnitTypes
} UnitType;

class Units {
public:
   Units(std::string const& filename);
   bool Identify();
   void Print();

   UnitEntry time;
   UnitEntry length;
   UnitEntry mass;
   UnitEntry charge;

   static cnb_float get(UnitType const& type, std::string const& name);

   typedef std::vector< std::vector<UnitEntry> > AllUnits;
   static AllUnits all;
};

} /* namespace cnb */

#endif
