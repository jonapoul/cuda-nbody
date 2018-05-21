#include "Config.h"

#include <sys/stat.h>
#include <math.h>
#include <cstring>
#include <string>
using namespace std;

#include "Constants.h"
#include "TeeStream.h"
#include "Units.h"
using namespace cnb;

extern "C" {
#include "PF.h"
}

Constants::Constants(string const& filename,
                     Units const& units) {
   /* check if the File exists */
   struct stat buffer;   
   if (stat(filename.c_str(), &buffer) != 0) {
      terr << "Constants File '" << filename << "'' doesn't exist\n";
      exit(1);
   }

#ifdef CNB_FLOAT
   DataType type = FLOAT;
#else
   DataType type = DOUBLE;
#endif
   size_t const NumParams = 10;
   PF_ParameterEntry * Params = new PF_ParameterEntry[NumParams];
   for (size_t i = 0; i < NumParams; i++) {
      Params[i].Type      = type;
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
      terr << "Failed to open constants File '" << filename << "'\n";
      exit(1);
   }

   /* Read the constants */
   if (PF_ReadParameterFile(File, Params, NumParams) != EXIT_SUCCESS) {
      terr << "Failed to read constants File '" << filename << "'\n";
      exit(1);
   }

   /* Clean up */
   delete[] Params;
   fclose(File);

   /* convert the units */
   cnb_float const t = units.time.val;
   cnb_float const m = units.mass.val;
   cnb_float const l = units.length.val;
   cnb_float const q = units.charge.val;
   c         *= ( t / l );
   e         *= ( q );
   G         *= ( m * t*t / (l*l*l) );
   h         *= ( t / (l*l * m) );
   k_b       *= ( (t*t) / (m * l*l)  );
   m_e       /= ( m );
   m_p       /= ( m );
   epsilon_0 *= ( m * l*l*l / (t*t * q*q) );
   mu_0      *= ( q*q / (m * l) );
   DEBUG_PRINT(c);

   this->hbar = this->h / (2*M_PI);
}

void Constants::Print() const {
   tee << "c         = " << c << '\n';
   tee << "e         = " << e << '\n';
   tee << "G         = " << G << '\n';
   tee << "h         = " << h << '\n';
   tee << "hbar      = " << hbar << '\n';
   tee << "k_b       = " << k_b << '\n';
   tee << "m_e       = " << m_e << '\n';
   tee << "m_p       = " << m_p << '\n';
   tee << "epsilon_0 = " << epsilon_0 << '\n';
   tee << "mu_0      = " << mu_0 << '\n';
}