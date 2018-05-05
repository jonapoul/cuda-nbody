#ifndef CUDANBODY_DEBUG_H
#define CUDANBODY_DEBUG_H

#include <string>
#include <fstream>

namespace cuda_nbody {

class DebugStream {
public:
   DebugStream();
   ~DebugStream();

   void SwitchTerminalOutput(bool const is_enabled);
   void SwitchFileOutput(bool const is_enabled);
   void SetFilename(std::string const& filename);
   
   template<typename T>
   friend std::ostream& operator<<(std::ostream&, T const&);

private:
   bool stdout_enabled  = true;
   bool file_enabled    = false;
   std::string filename = "";
   std::ofstream file;
};

/* GLOBAL OMG */
DebugStream output;

} // namespace cuda_nbody

#endif