#ifndef CUDANBODY_INTEGRATOR_H
#define CUDANBODY_INTEGRATOR_H

#include "Config.h"

namespace cnb {

class Integrator {
public:
   Integrator();
   Integrator(Integrator const&);
   
private:
   int x;
};

} /* namespace cnb */

#endif