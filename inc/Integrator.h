#ifndef CUDANBODY_INTEGRATOR_H
#define CUDANBODY_INTEGRATOR_H

#include <vector>
#include "Config.h"
#include "Particle3.h"
#include "Vector3.h"
#include "Constants.h"

namespace cnb {

class Integrator {
public:
   Integrator(std::vector<Particle3> * p = nullptr,
              Constants * c = nullptr);
   void Init();
   void ResetForces();
   void UpdatePositions(cnb_float const dt);
   void CalculateForces();
   void UpdateVelocities(cnb_float const dt);

private:
   std::vector<Particle3> * particles;
   Constants * constants;
   std::vector<Vector3> oldForce;
   std::vector<Vector3> newForce;
   std::vector<Vector3> meanForce;
};

} /* namespace cnb */

#endif