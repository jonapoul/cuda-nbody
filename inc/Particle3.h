#ifndef CUDANBODY_VECTOR3_H
#define CUDANBODY_VECTOR3_H

#include <vector>
#include "Vector3.h"

namespace cuda_nbody {

class Particle3 {
public:
   Particle3();


private:
   Vector3 r;
   Vector3 v;
};

} // namespace cuda_nbody

#endif