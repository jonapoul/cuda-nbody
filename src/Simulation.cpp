#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Simulation.h"
#include "TeeStream.h"
using namespace cuda_nbody;

Simulation::Simulation() { }

size_t Simulation::NumParticles() const { return particles.size(); }

void Simulation::ReadParticlesFromFile(std::string const& directory) {
   /* Read in the particles from the ephemerides directory */
   for (auto& file : fs::directory_iterator(directory)) {
      if (!fs::is_regular_file(file)) {
         continue;
      }
      Particle3 p;
      std::string path_string = file.path().string();
      if ( p.ReadFromFile(path_string) ) {
         this->particles.push_back(p);
         tee << p << '\n';
      }
   }

   /* Check if we've failed any */
   if (particles.size() != cnt) {
      tee << "Not all ephemerides were successfully read in!\n";
      exit(EXIT_FAILURE);
   }
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
