#include <sys/stat.h>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

#include "Units.h"
#include "TeeStream.h"
using namespace cnb;

extern "C" {
#include "PF.h"
}

vector<UnitEntry> Units::all_time = {
   { "fs",  1e-15 },
   { "ns",  1e-9 },
   { "s",   1 },
   { "min", 60 },
   { "hr",  3600 },
   { "day", 86400 },
   { "wk",  604800 },
   { "yr",  3.1556926e7 },
   { "kyr", 3.1556926e10 },
   { "Myr", 3.1556926e13 }
};
vector<UnitEntry> Units::all_length = {
   { "fm",  1e-15 },
   { "nm",  1e-9 },
   { "m",   1 },
   { "km",  1e3 },
   { "R_E", 6.371e6 },
   { "AU",  1.495e11 },
   { "ly",  9.46052e15 },
   { "pc",  3.08567758e16 },
   { "kpc", 3.08567758e19 },
   { "Mpc", 3.08567758e22 }
};
vector<UnitEntry> Units::all_mass = {
   { "m_e", 9.10938356e-31 },
   { "m_p", 1.6726219e-27 },
   { "fg",  1e-18 },
   { "ng",  1e-12 },
   { "g",   1e-3 },
   { "kg",  1 },
   { "ton", 1e3 },
   { "M_M", 7.34767309e22 },
   { "M_E", 5.97219e24 },
   { "M_S", 1.9891e30 }
};
vector<UnitEntry> Units::all_charge = {
   { "e", 1.60217662e-19 },
   { "C", 1 }
};

Units::Units(string const& filename) {
   /* check if the File exists */
   struct stat buffer;   
   if (stat(filename.c_str(), &buffer) != 0) {
      terr << "Units File '" << filename << "'' doesn't exist\n";
      exit(1);
   }

   size_t const NumParams = 4;
   PF_ParameterEntry * Params = new PF_ParameterEntry[NumParams];
   for (size_t i = 0; i < NumParams; i++) {
      Params[i].Type      = STRING;
      Params[i].IsBoolean = 0;
      Params[i].IsArray   = 0;
   }

   char timebuf[MAX_LINE_LENGTH];
   char lengthbuf[MAX_LINE_LENGTH];
   char massbuf[MAX_LINE_LENGTH];
   char chargebuf[MAX_LINE_LENGTH];

   strncpy(Params[0].Parameter,  "Time",     MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[1].Parameter,  "Distance", MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[2].Parameter,  "Mass",     MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[3].Parameter,  "Charge",   MAX_PARAMETER_NAME_LENGTH);
   Params[0].Pointer  = &timebuf;
   Params[1].Pointer  = &lengthbuf;
   Params[2].Pointer  = &massbuf;
   Params[3].Pointer  = &chargebuf;

   /* Open file for reading */
   FILE * File;
   if ( (File = fopen(filename.c_str(), "r")) == NULL) {
      terr << "Failed to open units File '" << filename << "'\n";
      exit(1);
   }

   /* Read the units */
   if (PF_ReadParameterFile(File, Params, NumParams) != EXIT_SUCCESS) {
      terr << "Failed to read units File '" << filename << "'\n";
      exit(1);
   }

   this->time.name   = std::string(timebuf);
   this->length.name = std::string(lengthbuf);
   this->mass.name   = std::string(massbuf);
   this->charge.name = std::string(chargebuf);

   /* Clean up */
   delete[] Params;
   fclose(File);

   /* identify time units */
   bool const AllUnitsFound = this->Identify();
   if (!AllUnitsFound) {
      tee << "One or more of the units weren't read in properly!\n";
      exit(1);
   }

}

/* Match up each of the read-in units to the static arrays of possible units */
bool Units::Identify() {
   for (auto t : all_time) {
      if (time.name == t.name) {
         time.val = t.val;
         tee << "Time units:     " << time.name << '\n';
         break;
      }
   }
   if (time.val < 0) return false;

   for (auto l : all_length) {
      if (length.name == l.name) {
         length.val = l.val;
         tee << "Distance units: " << length.name << '\n';
         break;
      }
   }
   if (length.val < 0) return false;

   for (auto m : all_mass) {
      if (mass.name == m.name) {
         mass.val = m.val;
         tee << "Mass units:     " << mass.name << '\n';
         break;
      }
   }
   if (mass.val < 0) return false;

   for (auto c : all_charge) {
      if (charge.name == c.name) {
         charge.val = c.val;
         tee << "Charge units:   " << charge.name << '\n';
         break;
      }
   }
   if (charge.val < 0) return false;

   return true;
}

void Units::Print() {
   tee << "Base time Units:   " << time.name << " = " << time.val << " s\n";
   tee << "Base Length Units: " << length.name << " = " << length.val << " s\n";
   tee << "Base Mass Units:   " << mass.name << " = " << mass.val << " s\n";
   tee << "Base Charge Units: " << charge.name << " = " << charge.val << " s\n";
}