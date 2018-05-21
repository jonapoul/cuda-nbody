#include <iostream>
#include <math.h>
#include <sstream>
#include <iterator>

/* Boost regex is used here just because std doesn't support positive
   lookbehinds, for whatever silly reason */
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Particle3.h"
#include "Simulation.h"
#include "TeeStream.h"

using namespace cnb;

Particle3::Particle3(Simulation * s) 
   : position({}), velocity({}), mass(0.0),
     radius(0.0), name(""), sim(s) { }

Particle3::Particle3(Particle3 const& p) {
  this->position = p.position;
  this->velocity = p.velocity;
  this->mass     = p.mass;
  this->radius   = p.radius;
  this->name     = p.name;
  this->sim      = p.sim;
}

cnb_float    Particle3::Mass()     const { return this->mass; }
cnb_float&   Particle3::Mass()           { return this->mass; }
cnb_float    Particle3::Radius()   const { return this->radius; }
cnb_float&   Particle3::Radius()         { return this->radius; }
Vector3      Particle3::Position() const { return this->position; }
Vector3&     Particle3::Position()       { return this->position; }
Vector3      Particle3::Velocity() const { return this->velocity; }
Vector3&     Particle3::Velocity()       { return this->velocity; }
std::string  Particle3::Name()     const { return this->name; }
std::string& Particle3::Name()           { return this->name; }

bool Particle3::ReadFromFile(std::string const& filename) {
   std::ifstream file(filename);
   if (!file.is_open())
      return false;

   /* Read the file contents into a stringstream and pass it to a string */
   std::stringstream instream;
   instream << file.rdbuf();
   file.close();
   std::string const file_text = instream.str();
   if (file_text.length() == 0)
      return false;

   if (!ReadName(file_text)) {
      tee << "name failed\n";
      return false;
   } else if (!ReadRadius(file_text)) {
      tee << "radius failed\n";
      return false;
   } else if (!ReadMass(file_text)) {
      tee << "mass failed\n";
      return false;
   } else if (!ReadPosition(file_text)) {
      tee << "position failed\n";
      return false;
   } else if (!ReadVelocity(file_text)) {
      tee << "velocity failed\n";
      return false;
   }
   return true;
}

bool Particle3::ReadName(std::string const& file) {
   std::vector<boost::regex> const patterns {
      boost::regex("(?<=Revised: \\w{3} \\d{2}, \\d{4})(\\s{1,}\\w{1,})"),
      boost::regex("(?<=Revised : \\w{3} \\d{2}, \\d{4})(\\s{1,}\\w{1,})"),
   };
   boost::sregex_iterator itr, end_itr;
   for (size_t i = 0; i < patterns.size(); ++i) {
      itr = boost::sregex_iterator(file.begin(), file.end(), patterns[i]);
      if (RegexMatchCount(itr) == 1) {
         this->name = itr->str();
         boost::algorithm::trim(name);
         name[0] = toupper(name[0]);
         return true;
      }
   }
   return false;
}

bool Particle3::ReadRadius(std::string const& file) {
   std::vector<boost::regex> const patterns {
      boost::regex("(?<=Radius \\(photosphere\\)  =)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mean radius \\(km\\)\\s{6}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mean Radius \\(km\\)\\s{7}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mean Radius \\(km\\)\\s{8}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mean radius, km\\s{10}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Radius \\(km\\)\\s{13}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Radius \\(km\\)\\s{12}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Radius \\(km\\)\\s{11}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Volumetric mean radius=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Radius \\(gravity\\), km  =)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Radius of Pluto, Rp   =)(\\s*\\d{1,}\\.*\\d*)"),
   };
   boost::sregex_iterator itr, end_itr;
   for (size_t i = 0; i < patterns.size(); ++i) {
      itr = boost::sregex_iterator(file.begin(), file.end(), patterns[i]);
      if (RegexMatchCount(itr) == 1) {
         this->radius = CNB_STOF(itr->str());
         radius *= (i == 0) ? 1e8 : 1e3;
         radius /= sim->units->mass.val;
         return true;
      }
   }
   return false;
}

bool Particle3::ReadMass(std::string const& file) {
   std::vector<boost::regex> const patterns {
      boost::regex("(?<=Mass \\(10\\^19 kg\\)\\s{8}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^19 kg\\)\\s{9}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^19 kg \\)\\s{8}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^20 kg \\)\\s{6}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^20 kg \\)\\s{8}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^21 kg \\)\\s{8}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^23 kg \\)\\s{8}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^23 kg \\)\\s{6}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^24 kg\\)\\s{7}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^26 kg\\)\\s{7}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass \\(10\\^30 kg\\)   ~)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass, 10\\^20 kg\\s{8}=)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass, 10\\^24 kg =)(\\s*\\d{1,}\\.*\\d*)"),
      boost::regex("(?<=Mass Pluto \\(10\\^22 kg\\) =)(\\s*\\d{1,}\\.*\\d*)"),
   };
   boost::sregex_iterator itr, itr2, end_itr;
   for (size_t i = 0; i < patterns.size(); ++i) {
      itr = boost::sregex_iterator(file.begin(), file.end(), patterns[i]);
      if (RegexMatchCount(itr) == 1) {
         this->mass = CNB_STOF(itr->str());

         /* find the scaling value from the above patterns */
         boost::regex scaling("(?<=10\\\\\\^)(\\d{1,})");
         std::string const current = patterns[i].str();
         itr2 = boost::sregex_iterator(current.begin(), current.end(), scaling);
         if (RegexMatchCount(itr2) == 1) {
            /* scale to units of earth mass */
            mass *= pow(10, stoi(itr2->str()));
            mass /= sim->units->mass.val;
         }
         return true;
      }
   }
   return false;
}

bool Particle3::ReadPosition(std::string const& file) {
   std::vector<boost::regex> const patterns {
      boost::regex("(?<=X =)((\\-| )\\d{1,}\\.*\\d*E(\\+|\\-)\\d{2})"),
      boost::regex("(?<=Y =)((\\-| )\\d{1,}\\.*\\d*E(\\+|\\-)\\d{2})"),
      boost::regex("(?<=Z =)((\\-| )\\d{1,}\\.*\\d*E(\\+|\\-)\\d{2})"),
   };
   boost::sregex_iterator itr, itr2, end_itr;

   itr = boost::sregex_iterator(file.begin(), file.end(), patterns[0]);
   if (RegexMatchCount(itr) == 0) return false;
   cnb_float x = CNB_STOF(itr->str());

   itr = boost::sregex_iterator(file.begin(), file.end(), patterns[1]);
   if (RegexMatchCount(itr) == 0) return false;
   cnb_float y = CNB_STOF(itr->str());

   itr = boost::sregex_iterator(file.begin(), file.end(), patterns[2]);
   if (RegexMatchCount(itr) == 0) return false;
   cnb_float z = CNB_STOF(itr->str());

   this->position = Vector3(x, y, z);
   this->position /= sim->units->length.val;
   return true;
}

bool Particle3::ReadVelocity(std::string const& file) {
   std::vector<boost::regex> const patterns {
      boost::regex("(?<=VX=)((\\-| )\\d{1,}\\.*\\d*E(\\+|\\-)\\d{2})"),
      boost::regex("(?<=VY=)((\\-| )\\d{1,}\\.*\\d*E(\\+|\\-)\\d{2})"),
      boost::regex("(?<=VZ=)((\\-| )\\d{1,}\\.*\\d*E(\\+|\\-)\\d{2})"),
   };
   boost::sregex_iterator itr, itr2, end_itr;

   itr = boost::sregex_iterator(file.begin(), file.end(), patterns[0]);
   if (RegexMatchCount(itr) == 0) return false;
   cnb_float vx = CNB_STOF(itr->str());

   itr = boost::sregex_iterator(file.begin(), file.end(), patterns[1]);
   if (RegexMatchCount(itr) == 0) return false;
   cnb_float vy = CNB_STOF(itr->str());

   itr = boost::sregex_iterator(file.begin(), file.end(), patterns[2]);
   if (RegexMatchCount(itr) == 0) return false;
   cnb_float vz = CNB_STOF(itr->str());

   this->velocity = Vector3(vx, vy, vz);
   this->velocity *= (sim->units->time.val / sim->units->length.val);
   return true;
}

size_t Particle3::RegexMatchCount(boost::sregex_iterator itr) {
   return std::distance(itr, boost::sregex_iterator());
}

bool Particle3::Compare(Particle3 const& p) const {
   return this->position.Compare( p.position )
       && this->velocity.Compare( p.velocity )
       && abs(this->mass   - p.mass)   / this->mass   < CNB_EPSILON
       && abs(this->radius - p.radius) / this->radius < CNB_EPSILON;
}

std::string Particle3::ToString() const {
   /* REDO THIS SO IT ACTUALLY LOOKS DECENT */
   std::stringstream ss;
   ss << name << ' ' << radius << ' ' << mass << ' ' << position << ' ' << velocity << '\n';
   return ss.str();
}

Particle3& Particle3::operator=(Particle3 const& p) {
   this->position = p.position;
   this->velocity = p.velocity;
   this->mass     = p.mass;
   this->radius   = p.radius;
   this->name     = p.name;
   return *this;
}

/* this namepsace scoping is to remove linking errors */
namespace cnb {

std::ostream& operator<<(std::ostream& os, Particle3 const& p) {
   size_t const bufsize = Simulation::LongestParticleName;
   if (p.Name().length() < bufsize) {
      os << std::string(bufsize-p.Name().length(), ' ');
   }
   os << p.Name() << ' ';
   os << "pos=" << p.Position() << ' ';
   os << "vel=" << p.Velocity() << ' ';

   char buf[64];
   snprintf(buf, 64, "mass=%.2e radius=%.2e", p.Mass(), p.Radius());
   os << buf;
   return os;
}

} /* namespace cnb */
