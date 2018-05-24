#ifndef CUDANBODY_TEESTREAM_H
#define CUDANBODY_TEESTREAM_H

#include <streambuf>
#include <fstream>
#include <string>
#include <iostream>

#include "functions.h"

namespace cnb {

/* Adapted from http://wordaligned.org/articles/cpp-streambufs */
template <typename char_type, typename traits = std::char_traits<char_type> >
class Basic_TeeBuf: public std::basic_streambuf<char_type, traits> {
public:
   typedef typename traits::int_type int_type;

   Basic_TeeBuf(std::basic_streambuf<char_type, traits> * buf1,
                std::basic_streambuf<char_type, traits> * buf2)
            : sb1(buf1), sb2(buf2) { }

private:
   virtual int sync() {
      int const r1 = sb1->pubsync();
      int const r2 = sb2->pubsync();
      return r1 == 0 && r2 == 0 ? 0 : -1;
   }

   virtual int_type overflow(int_type c) {
      int_type const eof = traits::eof();
      if (traits::eq_int_type(c, eof)) {
         return traits::not_eof(c);
      } else {
         char_type const ch = traits::to_char_type(c);
         int_type const r1 = sb1->sputc(ch);
         int_type const r2 = sb2->sputc(ch);
         return traits::eq_int_type(r1, eof) ||
                traits::eq_int_type(r2, eof) ? eof : c;
      }
   }

private:
   std::basic_streambuf<char_type, traits> * sb1;
   std::basic_streambuf<char_type, traits> * sb2;
};
typedef Basic_TeeBuf<char> TeeBuf;

class TeeStream : public std::ostream {
public:
   TeeStream() : std::ostream(&tbuf),
                 log(GenerateFilename()),
                 tbuf(std::cout.rdbuf(), log.rdbuf()) { }

   std::string GenerateFilename() const {
      return "logs/" + timestamp() + ".log";
   }
private:
   std::ofstream log;
   TeeBuf tbuf;
};

} // namespace cnb

/* Allocated in src/TeeStream.cpp */
extern cnb::TeeStream tee;

#define terr tee << __FUNCTION__ << " " << __LINE__ << ": "

#endif