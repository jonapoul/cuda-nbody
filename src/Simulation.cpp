#include <ctime>
#include <algorithm>
using namespace std;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Simulation.h"
#include "TeeStream.h"
#include "Constants.h"
#include "Units.h"
#include "Integrator.h"
using namespace cnb;

extern "C" {
#include "PF.h"
}

Simulation::Simulation() : integrator(nullptr) {
   units      = new Units("params/Units.param");
   constants  = new Constants("params/Constants.param", units);
}

Simulation::~Simulation() {
   traj.close();
   delete units;
   delete constants;
   delete integrator;
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
   size_t const NumParams = 4;
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
   strncpy(Params[0].Parameter, "Timestep",    MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[1].Parameter, "EndTime",     MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[2].Parameter, "Particles",   MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[3].Parameter, "PrintingGap", MAX_PARAMETER_NAME_LENGTH);
   Params[0].Pointer = &dt;
   Params[1].Pointer = &t_max;
   Params[2].Pointer = &count;         Params[2].Type = STRING;
   Params[3].Pointer = &printingGap;   Params[3].Type = INTEGER;

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
   tee << "Parameters:\n";
   tee << CNB_INDENT << "Timestep    = " << dt << " " << units->time.name << '\n';
   tee << CNB_INDENT << "EndTime     = " << t_max << " " << units->time.name << '\n';
   tee << CNB_INDENT << "PrintingGap = " << printingGap << '\n';
   if (string(count) != "All") {
      allowedParticleCount = stoi(count);
      tee << CNB_INDENT << "Particles   = " << allowedParticleCount << '\n';
   } else {
      allowedParticleCount = 0;
      tee << CNB_INDENT << "Particles   = All\n";
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
         LongestParticleName = CNB_MAX( p.Name().length(), LongestParticleName );
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
   if (allowedParticleCount > 0) {
      particles = vector<Particle3>(particles.begin(),
                                    particles.begin()+allowedParticleCount);
   }

   integrator = new Integrator(&particles, constants);
   integrator->CalculateForces();
}

void Simulation::DetermineOrbitalCentres() {
   struct Forces {
      cnb_float force;
      int index;
   };

   for (size_t i = 0; i < particles.size(); ++i) {
      Particle3 * p = &(particles[i]);
      /* Check if particle is abnormally massive */
      bool particleIsNotSun = false;
      vector<Forces> forces(NumParticles());
      for (size_t j = 0; j < NumParticles(); ++j) {
         if (!p->Compare(particles[j])) {
            if (p->Mass() < 1000*particles[j].Mass()) {
               particleIsNotSun = true;
            }
            forces[j].force = p->NetForce(particles[j]).Magnitude();
            forces[j].index = j;
         }
      }
      /* No orbit centre if so */
      if (!particleIsNotSun) {
         p->SetCentre(p);
         continue;
      }

      /* Sort in order of force magnitude */
      auto sorting = [](Forces& f1, Forces& f2) { return f1.force > f2.force; };
      sort(forces.begin(), forces.end(), sorting);

      /* Array holding the indices of all possible orbit centres */
      vector<size_t> orbitCentres;
      for (auto f : forces) {
         if (f.force > 0.01*forces[0].force) {
            orbitCentres.push_back(f.index);
         }
      }
      /* If more than one option, grab the one with the highest angular freq */
      if (orbitCentres.size() > 1) {
         cnb_float biggest = 0;
         for (auto centre : orbitCentres) {
            Particle3 const c = particles[centre];
            if (p->Mass() > c.Mass()) {
               continue;
            }
            cnb_float const r = (c.Position() - p->Position()).Magnitude();
            cnb_float const w = CNB_SQRT( c.Mass() / CNB_POW(r,3) );
            if (w > biggest) {
               biggest = w;
               p->SetCentre( &(particles[centre]) );
            }
         }
      } else {
         /* Otherwise, just use the particle applying the largest force */
         p->SetCentre( &(particles[orbitCentres[0]]) );
      }
   }
}

void Simulation::PrintParticles() const {
   tee << "Particles:\n";
   for (size_t i = 0; i < particles.size(); i++) {
      Particle3 const * const p = &(particles[i]);
      tee << CNB_INDENT << i+1 << (i+1 < 10 ? " " : "") << '/';
      tee << particles.size() << ": " << *p << "  ";
      tee << "Orbits " << p->Centre()->Name();
      tee << endl;
   }
}

void Simulation::PrintToTrajectoryFile(size_t const iteration) {
   traj << NumParticles() << '\n';
   traj << "Point = " << iteration << '\n';
   for (const auto& p : particles) {
      traj << p.Name() << ' ';
      if (p.Name().length() < LongestParticleName) {
         traj << string(LongestParticleName - p.Name().length(), ' ');
      }
      traj << p.Position() << '\n';
   }
}

void Simulation::Update() {
   integrator->ResetForces();
   integrator->UpdatePositions(dt);
   integrator->CalculateForces();
   integrator->UpdateVelocities(dt);
}
