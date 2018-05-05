#include <iostream>
#include <fstream>

#include "Config.h"
#include "Vector3.h"
#include "Particle3.h"
#include "DebugStream.h"

int main() {
   std::ofstream log_file( output.GenerateFilename() );
   output.LinkStream(log_file);
   output.LinkStream(std::cout);
   
}