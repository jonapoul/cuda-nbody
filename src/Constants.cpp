#include <sys/stat.h>
#include <cstring>
#include <string>
using namespace std;

#include "Constants.h"
#include "TeeStream.h"
using namespace cnb;

extern "C" {
#include "PF.h"
}

Constants::Constants(string const& Filename) {
   /* check if the File exists */
   struct stat buffer;   
   if (stat(Filename.c_str(), &buffer) != 0) {
      tee << "ERROR: Constants File '" << Filename << "'' doesn't exist\n";
      exit(1);
   }

#ifdef CNB_FLOAT
   DataType type = FLOAT;
#else
   DataType type = DOUBLE;
#endif
   size_t const NumParams = 12;
   PF_ParameterEntry * Params = new PF_ParameterEntry[NumParams];
   for (size_t i = 0; i < NumParams; i++) {
      Params[i].Type      = type;
      Params[i].IsBoolean = 0;
      Params[i].IsArray   = 0;
   }

   strncpy(Params[0].Parameter,  "c",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[1].Parameter,  "e",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[2].Parameter,  "G",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[3].Parameter,  "h",         MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[4].Parameter,  "hbar",      MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[5].Parameter,  "H_0",       MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[6].Parameter,  "k_b",       MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[7].Parameter,  "m_e",       MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[8].Parameter,  "m_p",       MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[9].Parameter,  "epsilon_0", MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[10].Parameter, "mu_0",      MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[11].Parameter, "sigma",     MAX_PARAMETER_NAME_LENGTH);
   Params[0].Pointer  = &(this->c);
   Params[1].Pointer  = &(this->e);
   Params[2].Pointer  = &(this->G);
   Params[3].Pointer  = &(this->h);
   Params[4].Pointer  = &(this->hbar);
   Params[5].Pointer  = &(this->H_0);
   Params[6].Pointer  = &(this->k_b);
   Params[7].Pointer  = &(this->m_e);
   Params[8].Pointer  = &(this->m_p);
   Params[9].Pointer  = &(this->epsilon_0);
   Params[10].Pointer = &(this->mu_0);
   Params[11].Pointer = &(this->sigma);

   /* Open file for reading */
   FILE * File;
   if ( (File = fopen(Filename.c_str(), "r")) == NULL) {
      tee << "ERROR: Failed to open constants File '" << Filename << "'\n";
      exit(1);
   }

   /* Read the constants */
   if (PF_ReadParameterFile(File, Params, NumParams) != EXIT_SUCCESS) {
      tee << "ERROR: Failed to read constants File '" << Filename << "'\n";
      exit(1);
   }

   /* Clean up */
   delete[] Params;
   fclose(File);
}