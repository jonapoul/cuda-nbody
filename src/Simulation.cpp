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
   std::string const filepath = directory + '/' + timestamp() + ".xyz";
   traj.open(filepath);
   if (!traj.is_open()) {
      terr << "Couldn't open trajectory file '" << filepath << "'\n";
      exit(1);
   } else {
      tee << "Successfully opened trajectory file '" << filepath << "'\n";
   }
}

void Simulation::ReadParameters(std::string const& filename) {
   // char pad_a[PAD_SIZE];
   size_t const NumParams = 3;
   PF_ParameterEntry * Params = new PF_ParameterEntry[NumParams];
   for (size_t i = 0; i < NumParams; i++) {
#ifdef CNB_FLOAT
      Params[i].Type      = FLOAT;
#else
      Params[i].Type      = DOUBLE;
#endif
      Params[i].IsBoolean = 0;
      Params[i].IsArray   = 0;
   }

   char count[MAX_LINE_LENGTH];
   strncpy(Params[0].Parameter, "Timestep",  MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[1].Parameter, "EndTime",   MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[2].Parameter, "Particles", MAX_PARAMETER_NAME_LENGTH);
   Params[0].Pointer = &dt;
   Params[1].Pointer = &t_max;
   Params[2].Pointer = &count;
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

   /* Rescale units */
   dt    *= (Units::get(TIME, "day") / units->time.val);
   t_max *= (Units::get(TIME, "day") / units->time.val);

   /* Print results */
   string const indent(CNB_INDENT, ' ');
   tee << "Parameters:\n";
   tee << indent << "Timestep  = " << dt << " " << units->time.name << '\n';
   tee << indent << "EndTime   = " << t_max << " " << units->time.name << '\n';
   if (string(count) != "All") {
      allowed_particle_count = stoi(count);
      tee << indent << "Particles = " << allowed_particle_count << '\n';
   } else {
      tee << indent << "Particles = All\n";
   }
   tee << flush;
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
         particles.push_back(p);
      } else {
         terr << "Failed reading '" << path_string << "'\n";
         exit(1);
      }
   }

   auto SortParticles = [](Particle3 const& p1, Particle3 const& p2) {
      return p1.Mass() > p2.Mass();
   };
   sort(particles.begin(), particles.end(), SortParticles);
   if (allowed_particle_count > 0) {
      particles = vector<Particle3>(particles.begin(),
                                    particles.begin()+allowed_particle_count);
   }
   tee << "Particles:\n";
   for (size_t i = 0; i < particles.size(); i++) {
      tee << i+1 << '/' << particles.size() << '\n';
      tee << particles[i] << '\n';
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

void Simulation::DetermineOrbitalCentres() {
   struct Forces {
      cnb_float force;
      int index;
   };

   for (auto& p : particles) {
      /* Check if particle is abnormally massive */
      bool particleIsNotSun = false;
      vector<Forces> forces(NumParticles());
      for (size_t i = 0; i < NumParticles(); ++i) {
         if (!p.Compare(particles[i])) {
            if (p.Mass() < 1000*particles[i].Mass()) {
               particleIsNotSun = true;
            }
            forces[i].force = p.NetForce(particles[i]).Magnitude();
            forces[i].index = i;
         }
      }
      /* No orbit centre if so */
      if (!particleIsNotSun) {
         p.SetCentre(nullptr);
         continue;
      }

      /* Sort in order of force magnitude */
      auto sorting = [](Forces& f1, Forces& f2) { return f1.force > f2.force; };
      sort(forces.begin(), forces.end(), sorting);

      /* Array holding the indices of all possible orbit centres */
      vector<size_t> orbitCentres;
      for (auto f : forces) {
         if (f.force > 0.5*forces[0].force) {
            orbitCentres.push_back(f.index);
         }
      }
      /* If there's more than one option, grab the one with the lowest mass */
      if (orbitCentres.size() > 1) {
         int minIndex = 0;
         cnb_float minMass = CNB_FLOAT_MAX;
         for (auto centre : orbitCentres) {
            if (particles[centre].Mass() < minMass) {
               minMass  = particles[centre].Mass();
               minIndex = centre;
            }
         }
         p.SetCentre( &(particles[minIndex]) );
      } else {
         /* Otherwise, just use the particle applying the largest force */
         p.SetCentre( &(particles[orbitCentres[0]]) );
      }
      tee << padded(p.Name(), LongestParticleName) << " orbits " << p.Centre()->Name() << '\n';
   }
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
