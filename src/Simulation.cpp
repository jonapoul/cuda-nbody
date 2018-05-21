#include <algorithm>
using namespace std;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Simulation.h"
#include "TeeStream.h"
#include "Constants.h"
#include "Units.h"
using namespace cnb;

size_t Simulation::LongestParticleName = 0;

Simulation::Simulation(Units * u,
                       Constants * c) 
      : units(u), constants(c) {
   /* blank */
}

size_t Simulation::NumParticles() const {
   return particles.size();
}

void Simulation::ReadParticlesFromDirectory(string const& directory) {
   size_t FileCount = 0;
   /* Read in the particles from the ephemerides directory */
   for (auto& file : fs::directory_iterator(directory)) {
      if (!fs::is_regular_file(file)) {
         continue;
      }
      FileCount++;
      Particle3 p(this);
      string const path_string = file.path().string();
      if ( p.ReadFromFile(path_string) ) {
         LongestParticleName = MAX( p.Name().length(), LongestParticleName );
         this->particles.push_back(p);
      } else {
         terr << "Failed reading '" << path_string << "'\n";
         exit(1);
      }
   }

   auto SortParticles = [](Particle3 const& p1, Particle3 const& p2) {
      return p1.Name() < p2.Name();
   };
   sort(particles.begin(), particles.end(), SortParticles);
   for (auto& p : particles) {
      tee << p << '\n';
   }
   tee << NumParticles() << "/" << FileCount << " ephemerides read in.\n";
}

void Simulation::ConvertInitialConditionUnits() {
   int x;
}

void Simulation::UpdateParticlePositions() {
   int x;
}

void Simulation::UpdateParticleVelocities() {
   int x;
}

void Simulation::UpdateParticleForces() {
   int x;
}

void Simulation::CalculateInitialForces() {
   int x;
}

void Simulation::DetermineOrbitalCentres() {
   int x;
}
