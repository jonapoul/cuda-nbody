#ifndef CUDANBODY_INTEGRATOR_H
#define CUDANBODY_INTEGRATOR_H

#include "Config.h"

namespace cuda_nbody {

class Integrator {
public:
   Integrator();
   Integrator(Integrator const&);
   
private:
   int x;
};

} // namespace cuda_nbody

#endif