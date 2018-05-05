#ifndef CUDANBODY_VECTOR3_H
#define CUDANBODY_VECTOR3_H

#include <vector>
#include "Config.h"

namespace cuda_nbody {

class Vector3 {
public:
   Vector3(cnb_float const X = 0.0,
           cnb_float const Y = 0.0,
           cnb_float const Z = 0.0);

private:
   cnb_float x;
   cnb_float y;
   cnb_float z;
};

} // namespace cuda_nbody

#endif