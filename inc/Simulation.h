#ifndef CUDANBODY_SIMULATION_H
#define CUDANBODY_SIMULATION_H

#include <vector>
#include <fstream>

#include "Config.h"
#include "Particle3.h"

namespace cnb {

class Units;
class Constants;
class Integrator;
class Simulation {
public:
   Simulation();
   Simulation(Simulation const& s) = delete;
   Simulation& operator=(Simulation const& s) = delete;
   ~Simulation();

   void OpenTrajectoryFile(std::string const& directory);
   size_t NumParticles() const;

   void ReadParameters(std::string const& filename);
   void ReadParticlesFromDirectory(std::string const& directory);
   void DetermineOrbitalCentres();
   void PrintParticles() const;
   void PrintToTrajectoryFile(size_t const iteration);
   void Update();

   size_t LongestParticleName = 0;
   Units * units;
   Constants * constants;

   cnb_float dt    = -1;
   cnb_float t_max = -1;

private:
   std::vector<Particle3> particles;
   std::ofstream traj;
   size_t allowedParticleCount;
   Integrator * integrator;
};

} /* namespace cnb */

#endif