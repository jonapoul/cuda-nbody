#include <iostream>

#include "Config.h"
#include "Simulation.h"
#include "TeeStream.h"
using namespace cnb;

int main() {
   Units units("Units.param");
   Constants constants("Constants.param", units);
   Simulation sim(&units, &constants);
   sim.ReadParticlesFromDirectory("ephemerides");
   sim.ConvertInitialConditionUnits();

   sim.CalculateInitialForces();
   sim.DetermineOrbitalCentres();

   return EXIT_SUCCESS;
}