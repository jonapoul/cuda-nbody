#ifndef CUDANBODY_SIMULATION_H
#define CUDANBODY_SIMULATION_H

#include <vector>
#include <fstream>

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
   ~Simulation();

   void OpenTrajectoryFile(std::string const& directory);
   size_t NumParticles() const;

   void ReadParameters(std::string const& filename);
   void ReadParticlesFromDirectory(std::string const& directory);
   void UpdatePositions();
   void UpdateVelocities();
   void UpdateForces();
   void CalculateInitialForces();
   void DetermineOrbitalCentres();
   void PrintToTrajectoryFile(size_t const timestep);

   size_t LongestParticleName = 0;
   Units * units;
   Constants * constants;
   cnb_float dt;
   cnb_float t_max;

private:
   std::vector<Particle3> particles;
   std::ofstream traj;
};

} // namespace cnb

#endif