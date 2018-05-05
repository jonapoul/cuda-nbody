#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "DebugStream.h"

using namespace cuda_nbody;

DebugStream::DebugStream() { }

DebugStream::~DebugStream() {
   if (this->file_enabled) {
      this->file.close();
   }
}

void DebugStream::SwitchTerminalOutput(bool const is_enabled) {
   this->stdout_enabled = is_enabled;
}

void DebugStream::SwitchFileOutput(bool const is_enabled) {

}

void DebugStream::SetFilename(std::string const& filename) {
   this->filename = filename;
   struct stat buffer;   
   bool const file_exists = (stat(filename.c_str(), &buffer) == 0);
   
   if (file_exists)
      this->file.open(filename);
}

template<typename T>
DebugStream& operator<<(DebugStream& ds,
                        T const& argument) {
   if (ds.stdout_enabled) std::cout << argument;
   if (ds.file_enabled)   ds.file << argument;
   return ds;
}