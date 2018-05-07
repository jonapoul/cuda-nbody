#include <fstream>
#include <algorithm>
#include <functional>
#include <ctime>

#include "TeeStream.h"
using namespace cuda_nbody;

TeeStream::TeeStream() 
      : std::ostream(NULL) {
   std::ostream::rdbuf(&stream);
}

void TeeStream::LinkStream(std::ostream& out) {
   out.flush();
   stream.AddBuffer(out.rdbuf());
}

std::string TeeStream::GenerateFilename() const {
   time_t rawtime;
   struct tm * timeinfo;
   char buffer[32];
   time(&rawtime);
   timeinfo = localtime(&rawtime);
   strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);
   return "logs/" + std::string(buffer) + ".log";
}

int TeeStream::DebugBuffer::overflow(int c) {
   std::for_each(bufs.begin(),
                 bufs.end(),
                 std::bind2nd(std::mem_fun(&std::streambuf::sputc), c));
   return c;
}

void TeeStream::DebugBuffer::AddBuffer(std::streambuf* buf) {
   bufs.push_back(buf);
}