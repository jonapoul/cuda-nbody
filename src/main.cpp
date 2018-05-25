#include <iostream>

#include "Config.h"
#include "Simulation.h"
#include "TeeStream.h"
#include "Timer.h"
using namespace cnb;

int main() {
   Timer timer;

   Simulation sim;
   sim.ReadParameters("params/Simulation.param");
   sim.ReadParticlesFromDirectory("eph");
   sim.DetermineOrbitalCentres();
   sim.PrintParticles();
   sim.OpenTrajectoryFile("traj");
   
   tee << "Setup time = " << timer.elapsed() << " s\n";
   timer.reset();

   cnb_float t = 0;
   tee << "Number of iterations = " << (sim.t_max / sim.dt) << '\n';

   for (size_t iteration = 0; t <= sim.t_max; ++iteration) {
      if (iteration % sim.printingGap == 0) {
         sim.PrintToTrajectoryFile(iteration/sim.printingGap);
      }

      sim.Update();
      t += sim.dt;

      if (iteration % sim.printingGap == 0) {
         cnb_float const walltime = timer.elapsed();
         cnb_float const timeLeft = walltime * (sim.t_max / t) - walltime;
         printf("%.2f%% done, %s left    \r",
                100.0*t/sim.t_max, secsToString(timeLeft).c_str());
      }
   }

   tee << "\nTotal runtime = " << timer.elapsed() << " s   \n";
   return EXIT_SUCCESS;
}
