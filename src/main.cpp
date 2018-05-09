#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Config.h"
#include "Vector3.h"
#include "Particle3.h"
#include "TeeStream.h"
using namespace cuda_nbody;

TeeStream tee;

int main() {
   /* Initialised the double output streams */
   std::ofstream log_file( tee.GenerateFilename() );
   tee.LinkStream(log_file);
   tee.LinkStream(std::cout);

   /* Read in the particles from the ephemerides directory */
   fs::path path("ephemerides");
   int cnt = std::count_if(fs::directory_iterator(path),
                           fs::directory_iterator(),
                           static_cast<bool(*)(const fs::path&)>(fs::is_regular_file));
   tee << cnt << '\n';
   for (auto& file : fs::directory_iterator(path)) {
      Particle3 p;
      p.ReadFromFile(file.path().string());
      tee << p << '\n';
   }
}