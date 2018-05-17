#ifndef CUDANBODY_SIMULATION_H
#define CUDANBODY_SIMULATION_H

#include <vector>
#include "Config.h"
#include "Particle3.h"

namespace cuda_nbody {

class Simulation {
public:
   Simulation();
   Simulation(Simulation const&) = delete;
   Simulation& operator=(Simulation const&) = delete;

   size_t NumParticles() const;

   void ReadParticlesFromFile(std::string const& directory);
   void UpdateParticlePositions();
   void UpdateParticleVelocities();
   void UpdateParticleForces();

   void CalculateInitialForces();
   void DetermineOrbitalCentres();

private:
   std::vector<Particle3> particles;
};

} // namespace cuda_nbody

#endif