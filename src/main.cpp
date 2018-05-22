#include <iostream>

#include "Config.h"
#include "Simulation.h"
#include "TeeStream.h"
using namespace cnb;

int main() {
   Units units("params/Units.param");
   Constants constants("params/Constants.param", units);

   Simulation sim(&units, &constants);
   sim.ReadParticlesFromDirectory("eph");
   sim.ReadParameters("params/Simulation.param");
   sim.CalculateInitialForces();
   sim.DetermineOrbitalCentres();
   sim.OpenTrajectoryFile("traj");

   cnb_float t = 0;

   for (size_t iTimestep = 0; t < sim.t_max; ++iTimestep) {
      sim.PrintToTrajectoryFile(iTimestep);
      sim.UpdateForces();
      t += sim.dt;
   }

   return EXIT_SUCCESS;
}