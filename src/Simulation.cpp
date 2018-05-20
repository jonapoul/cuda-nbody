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

void Simulation::ReadParticlesFromDirectory(std::string const& directory) {
   size_t FileCount = 0;
   /* Read in the particles from the ephemerides directory */
   for (auto& file : fs::directory_iterator(directory)) {
      if (!fs::is_regular_file(file)) {
         continue;
      }
      FileCount++;
      Particle3 p;
      std::string const path_string = file.path().string();
      if ( p.ReadFromFile(path_string) ) {
         if (p.Name().length() > Simulation::LongestParticleName) {
            Simulation::LongestParticleName = p.Name().length();
         }
         this->particles.push_back(p);
      } else {
         tee << "Failed reading '" << path_string << "'\n";
      }
   }
   for (auto& p : particles) {
      tee << p << '\n';
   }
   tee << NumParticles() << "/" << FileCount
       << " ephemerides successfully read in.\n";
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
