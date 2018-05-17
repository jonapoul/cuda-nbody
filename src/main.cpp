#include <iostream>

#include "Config.h"
#include "Simulation.h"
#include "TeeStream.h"
using namespace cuda_nbody;

int main() {
   Simulation simulation;
   simulation.ReadParticlesFromFile("ephemerides");
   tee << simulation.NumParticles() << " ephemerides successfully read in.\n";
   simulation.CalculateInitialForces();
   simulation.DetermineOrbitalCentres();
   return EXIT_SUCCESS;
}