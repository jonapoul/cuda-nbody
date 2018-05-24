#include <sys/stat.h>
#include <math.h>
#include <cstring>
#include <string>
using namespace std;

#include "Constants.h"
#include "Config.h"
#include "TeeStream.h"
#include "Units.h"
#include "functions.h"
using namespace cnb;

extern "C" {
#include "PF.h"
}

Constants::Constants(string const& filename,
                     Units * u)
         : units(u) {
   /* check if the File exists */
   struct stat buffer;   
   if (stat(filename.c_str(), &buffer) != 0) {
      terr << "Constants File '" << filename << "'' doesn't exist\n";
      exit(1);
   }
   /* do the deed */
   ReadFromFile(filename);
   ConvertUnits();
   Print();
}

void Constants::Print() const {
   tee << "Constants:\n";
   string const t = units->time.name;
   string const l = units->length.name;
   string const m = units->mass.name;
   string const q = units->charge.name;
   string const indent(CNB_INDENT, ' ');
   tee << indent << "c    = " << c << ' '<< l << ' ' << t << "^-1\n";
   tee << indent << "e    = " << e << ' '<< q << '\n';
   tee << indent << "G    = " << G << ' ' << l << "^3 " << t << "^-2 " << m << "^-1\n";
   tee << indent << "h    = " << h << ' ' << l << "^2 " << m << " " << t << "^-1\n";
   tee << indent << "hbar = " << hbar << ' ' << l << "^2 " << m << " " << t << "^-1\n";
   tee << indent << "k_b  = " << k_b << ' ' << l << "^2 " << m << " " << t << "^-2\n";
   tee << indent << "m_e  = " << m_e << ' ' << m << '\n';
   tee << indent << "m_p  = " << m_p << ' ' << m << '\n';
   tee << indent << "\u03B5_0  = " << epsilon_0 << ' ' << q << "^2 " << t << "^2 " << m << "^-1 " << l << "^3\n";
   tee << indent << "\u03BC_0  = " << mu_0 << ' ' << m << ' ' << l << ' ' << q << "^-2\n"; 
   tee << flush;
}

void Constants::ReadFromFile(std::string const& filename) {
   size_t const NumParams = 10;
   PF_ParameterEntry * Params = new PF_ParameterEntry[NumParams];
   for (size_t i = 0; i < NumParams; i++) {
      Params[i].Type      = DOUBLE;
      Params[i].IsBoolean = 0;
      Params[i].IsArray   = 0;
   }

   strncpy(Params[0].Parameter, "c",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[1].Parameter, "e",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[2].Parameter, "G",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[3].Parameter, "h",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[4].Parameter, "hbar",      MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[5].Parameter, "k_b",       MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[6].Parameter, "m_e",       MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[7].Parameter, "m_p",       MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[8].Parameter, "epsilon_0", MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[9].Parameter, "mu_0",      MAX_PARAMETER_NAME_LENGTH);
   Params[0].Pointer = &(c);
   Params[1].Pointer = &(e);
   Params[2].Pointer = &(G);
   Params[3].Pointer = &(h);
   Params[4].Pointer = &(hbar);
   Params[5].Pointer = &(k_b);
   Params[6].Pointer = &(m_e);
   Params[7].Pointer = &(m_p);
   Params[8].Pointer = &(epsilon_0);
   Params[9].Pointer = &(mu_0);

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
}

void Constants::ConvertUnits() {
   double const t = units->time.val;
   double const m = units->mass.val;
   double const l = units->length.val;
   double const q = units->charge.val;
   c         *= ( t / l );
   e         *= ( q );
   G         *= ( m*t*t / (l*l*l) );
   h         *= ( t / (l*l*m) );
   k_b       *= ( (t*t) / (m*l*l)  );
   m_e       /= ( m );
   m_p       /= ( m );
   epsilon_0 *= ( m*l*l*l / (t*t*q*q) );
   mu_0      *= ( q*q / (m*l) );

   this->hbar = this->h / (2*M_PI);

   CHECK_RANGE(c,         DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(e,         DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(G,         DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(h,         DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(k_b,       DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(m_e,       DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(m_p,       DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(epsilon_0, DOUBLE_MIN, DOUBLE_MAX);
   CHECK_RANGE(mu_0,      DOUBLE_MIN, DOUBLE_MAX);
}
