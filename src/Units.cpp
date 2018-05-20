#include <sys/stat.h>
#include <cstring>
#include <string>
using namespace std;

#include "Units.h"
#include "TeeStream.h"
using namespace cnb;

extern "C" {
#include "PF.h"
}

Units::Units(string const& Filename) {
   /* check if the File exists */
   struct stat buffer;   
   if (stat(Filename.c_str(), &buffer) != 0) {
      tee << "ERROR: Units File '" << Filename << "'' doesn't exist\n";
      exit(1);
   }

#ifdef CNB_FLOAT
   DataType type = FLOAT;
#else
   DataType type = DOUBLE;
#endif
   size_t const NumParams = 3;
   PF_ParameterEntry * Params = new PF_ParameterEntry[NumParams];
   for (size_t i = 0; i < NumParams; i++) {
      Params[i].Type      = type;
      Params[i].IsBoolean = 0;
      Params[i].IsArray   = 0;
   }

   strncpy(Params[0].Parameter,  "Time",     MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[1].Parameter,  "Distance", MAX_PARAMETER_NAME_LENGTH);
   strncpy(Params[2].Parameter,  "Mass",     MAX_PARAMETER_NAME_LENGTH);
   Params[0].Pointer  = &(this->time.val);
   Params[1].Pointer  = &(this->distance.val);
   Params[2].Pointer  = &(this->mass.val);

   /* Open file for reading */
   FILE * File;
   if ( (File = fopen(Filename.c_str(), "r")) == NULL) {
      tee << "ERROR: Failed to open units File '" << Filename << "'\n";
      exit(1);
   }

   /* Read the units */
   if (PF_ReadParameterFile(File, Params, NumParams) != EXIT_SUCCESS) {
      tee << "ERROR: Failed to read units File '" << Filename << "'\n";
      exit(1);
   }

   /* Clean up */
   delete[] Params;
   fclose(File);
}