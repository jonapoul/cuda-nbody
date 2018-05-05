#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <ctime>

#include "DebugStream.h"
using namespace cuda_nbody;

DebugStream output;

DebugStream::DebugStream() 
      : std::ostream(NULL) {
   std::ostream::rdbuf(&myBuffer);
}

void DebugStream::LinkStream(std::ostream& out) {
   out.flush();
   myBuffer.AddBuffer(out.rdbuf());
}

std::string DebugStream::GenerateFilename() const {
   time_t rawtime;
   struct tm * timeinfo;
   char buffer[80];
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);
   return "logs/" + std::string(buffer) + ".log";
}