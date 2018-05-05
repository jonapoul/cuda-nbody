#ifndef CUDANBODY_DEBUG_H
#define CUDANBODY_DEBUG_H

#include <streambuf>
#include <string>
#include <vector>
#include <algorithm>

namespace cuda_nbody {

/* From https://stackoverflow.com/a/1761027 */
class DebugStream : public std::ostream {
public: 
   DebugStream();
   void LinkStream(std::ostream& out);
   std::string GenerateFilename() const;

private:
   struct DebugBuffer : public std::streambuf {
      void AddBuffer(std::streambuf* buf) {
         bufs.push_back(buf);
      }
      virtual int overflow(int c) {
         std::for_each(bufs.begin(),
                       bufs.end(),
                       std::bind2nd(std::mem_fun(&std::streambuf::sputc),c));
         return c;
      }
   private:
      std::vector<std::streambuf*> bufs;
   };
   DebugBuffer myBuffer;
};

} // namespace cuda_nbody

extern cuda_nbody::DebugStream output;

#endif