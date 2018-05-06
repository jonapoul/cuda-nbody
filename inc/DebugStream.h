#ifndef CUDANBODY_DEBUGSTREAM_H
#define CUDANBODY_DEBUGSTREAM_H

#include <streambuf>
#include <string>
#include <vector>

namespace cuda_nbody {

/* From https://stackoverflow.com/a/1761027 */
class DebugStream : public std::ostream {
public: 
   DebugStream();
   void LinkStream(std::ostream& out);
   std::string GenerateFilename() const;

private:
   struct DebugBuffer : public std::streambuf {
      void AddBuffer(std::streambuf* buf);
      virtual int overflow(int c);
      std::vector<std::streambuf*> bufs;
   };
   DebugBuffer stream;
};

} // namespace cuda_nbody

extern cuda_nbody::DebugStream output;

#endif