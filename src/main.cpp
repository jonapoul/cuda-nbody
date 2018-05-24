#include <iostream>

#include "Config.h"
#include "Simulation.h"
#include "TeeStream.h"
#include "Timer.h"
using namespace cnb;

int main() {
   Timer timer;
   Units units("params/Units.param");
   Constants constants("params/Constants.param", &units);

   Simulation sim(&units, &constants);
   sim.ReadParameters("params/Simulation.param");
   sim.ReadParticlesFromDirectory("eph");
   sim.DetermineOrbitalCentres();
   sim.PrintParticles();
   sim.OpenTrajectoryFile("traj");

   cnb_float t = 0;

   for (size_t iTimestep = 0; t < sim.t_max; ++iTimestep) {
      sim.PrintToTrajectoryFile(iTimestep);
      sim.UpdateForces();
      sim.UpdatePositions();
      sim.UpdateVelocities();
      t += sim.dt;
   }

   tee << "Total runtime = " << timer.elapsed() << "s\n";
   return EXIT_SUCCESS;
}
