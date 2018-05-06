#include <fstream>
#include <algorithm>
#include <functional>
#include <ctime>

#include "DebugStream.h"
using namespace cuda_nbody;

/* Global output stream */
DebugStream output;

DebugStream::DebugStream() 
      : std::ostream(NULL) {
   std::ostream::rdbuf(&stream);
}

void DebugStream::LinkStream(std::ostream& out) {
   out.flush();
   stream.AddBuffer(out.rdbuf());
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

int DebugStream::DebugBuffer::overflow(int c) {
   std::for_each(bufs.begin(),
                 bufs.end(),
                 std::bind2nd(std::mem_fun(&std::streambuf::sputc), c));
   return c;
}

void DebugStream::DebugBuffer::AddBuffer(std::streambuf* buf) {
   bufs.push_back(buf);
}