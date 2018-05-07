#include <iostream>
#include <fstream>

#include "Config.h"
#include "Vector3.h"
#include "Particle3.h"
#include "TeeStream.h"

#include <folly/Benchmark.h>
#include <folly/container/Foreach.h>
using namespace folly;
using namespace cuda_nbody;

size_t const NumRuns = 1e3;

BENCHMARK(EachLoop) {
   FOR_EACH_RANGE(i, 0, NumRuns) {
      time_t rawtime;
      struct tm * timeinfo;
      char buffer[32];
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
      std::string temp(buffer);
   }
}
BENCHMARK(Once) {
   time_t rawtime;
   struct tm * timeinfo;
   char buffer[32];
   FOR_EACH_RANGE(i, 0, NumRuns) {
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
      std::string temp(buffer);
   }
}

/* Global output stream, declared in TeeStream.h and allocated here */
TeeStream tee;

int main() {
   std::ofstream log_file( tee.GenerateFilename() );
   tee.LinkStream(log_file);
   tee.LinkStream(std::cout);

   runBenchmarks();

   tee << "test\n";
   return 0;
}