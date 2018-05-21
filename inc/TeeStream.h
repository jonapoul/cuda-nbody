#ifndef CUDANBODY_TEESTREAM_H
#define CUDANBODY_TEESTREAM_H

#include <streambuf>
#include <fstream>
#include <string>
#include <vector>

namespace cnb {

/* From https://stackoverflow.com/a/1761027 */
class TeeStream : public std::ostream {
public: 
   TeeStream();
   void LinkStream(std::ostream& out);
   std::string GenerateFilename() const;

private:
   struct DebugBuffer : public std::streambuf {
      void AddBuffer(std::streambuf * buf);
      virtual int overflow(int c);
      std::vector<std::streambuf *> bufs;
   };
   DebugBuffer stream;
   std::ofstream file;
};

} // namespace cnb

/* Allocated in src/main.cpp */
extern cnb::TeeStream tee;

#define terr tee << __FUNCTION__ << " " << __LINE__ << ": "

#endif