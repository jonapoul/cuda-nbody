#ifndef CUDANBODY_SIMULATION_H
#define CUDANBODY_SIMULATION_H

#include <vector>
#include "Config.h"
#include "Particle3.h"
#include "Constants.h"
#include "Units.h"

namespace cnb {

class Simulation {
public:
   Simulation(Units * u,
              Constants * c);
   Simulation(Simulation const& s) = delete;
   Simulation& operator=(Simulation const& s) = delete;

   size_t NumParticles() const;

   void ReadParticlesFromDirectory(std::string const& directory);
   void ConvertInitialConditionUnits();
   void UpdateParticlePositions();
   void UpdateParticleVelocities();
   void UpdateParticleForces();
   void CalculateInitialForces();
   void DetermineOrbitalCentres();

   static size_t LongestParticleName;

   Units * units;
   Constants * constants;

private:
   std::vector<Particle3> particles;
};

} // namespace cnb

#endif