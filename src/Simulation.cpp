#include <ctime>
#include <algorithm>
using namespace std;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Simulation.h"
#include "TeeStream.h"
#include "Constants.h"
#include "Units.h"
using namespace cnb;

extern "C" {
#include "PF.h"
}

Simulation::Simulation(Units * u,
                       Constants * c) 
      : units(u), constants(c) {
   /* blank */
}

Simulation::~Simulation() {
   traj.close();
   tee << flush;
}

size_t Simulation::NumParticles() const {
   return particles.size();
}

void Simulation::OpenTrajectoryFile(string const& directory) {
   time_t rawtime;
   struct tm * timeinfo;
   char filename[32];
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   strftime(filename, sizeof(filename), "%Y%m%d_%H%M%S", timeinfo);
   std::string const filepath = directory + '/' + string(filename) + ".xyz";
   traj.open(filepath);
   if (!traj.is_open()) {
      terr << "Couldn't open trajectory file '" << filename << "'\n";
      exit(1);
   }
}

void Simulation::ReadParameters(std::string const& filename) {
   size_t const NumParams = 3;
#ifdef CNB_FLOAT
   DataType type = FLOAT;
#else
   DataType type = DOUBLE;
#endif
   PF_ParameterEntry * Params = new PF_ParameterEntry[NumParams];
   for (size_t i = 0; i < NumParams; i++) {
      Params[i].Type      = type;
      Params[i].IsBoolean = 0;
      Params[i].IsArray   = 0;
   }

   char ParticlesToKeep_str[MAX_PARAMETER_NAME_LENGTH];

   strncpy(Params[0].Parameter, "Timestep",  MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[1].Parameter, "EndTime",   MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[2].Parameter, "Particles", MAX_PARAMETER_NAME_LENGTH);
   Params[0].Pointer = &dt;
   Params[1].Pointer = &t_max;
   Params[2].Pointer = &ParticlesToKeep_str;
   Params[2].Type = STRING;

   /* Open file for reading */
   FILE * File;
   if ( (File = fopen(filename.c_str(), "r")) == NULL) {
      terr << "Failed to open constants file '" << filename << "'\n";
      exit(1);
   }

   /* Read the constants */
   if (PF_ReadParameterFile(File, Params, NumParams) != EXIT_SUCCESS) {
      terr << "Failed to read constants file '" << filename << "'\n";
      exit(1);
   }

   /* Clean up */
   delete[] Params;
   fclose(File);

   tee << "Timestep  = " << dt << '\n';
   tee << "EndTime   = " << t_max << '\n';

   if (string(ParticlesToKeep_str) != "All") {
      int const ParticlesToKeep = stoi(ParticlesToKeep_str);
      tee << "Particles = " << ParticlesToKeep << '\n';
      for (size_t i = ParticlesToKeep; i < particles.size(); ++i) {
         particles.erase(particles.begin() + i);
      }
   } else {
      tee << "Particles = All\n";
   }

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
      return p1.Mass() > p2.Mass();
   };
   sort(particles.begin(), particles.end(), SortParticles);
   string const indent(CNB_INDENT, ' ');
   tee << "Particles:\n";
   for (auto& p : particles) {
      tee << indent << p << '\n';
   }
   tee << flush;
}

void Simulation::UpdatePositions() {
   int x;
}

void Simulation::UpdateVelocities() {
   int x;
}

void Simulation::UpdateForces() {
   int x;
}

void Simulation::CalculateInitialForces() {
   int x;
}

void Simulation::DetermineOrbitalCentres() {
   int x;
}

void Simulation::PrintToTrajectoryFile(size_t const timestep) {
   traj << NumParticles() << '\n';
   traj << "Point = " << timestep << '\n';
   for (const auto& p : particles) {
      traj << p.Name() << ' ';
      if (p.Name().length() < LongestParticleName) {
         traj << string(LongestParticleName - p.Name().length(), ' ');
      }
      traj << p.Position() << '\n';
   }
}
