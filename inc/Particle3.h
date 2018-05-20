#ifndef CUDANBODY_PARTICLE3_H
#define CUDANBODY_PARTICLE3_H

#include <vector>
#include <string>
#include <fstream>

#include <boost/regex.hpp>

#include "Config.h"
#include "Vector3.h"

namespace cnb {

typedef enum {
   EULER,
   RK2,
   RK4
} IntegrationType;

class Particle3 {
public:
   Particle3();
   Particle3(Particle3 const& p);
   Particle3& operator=(Particle3 const& p);

   bool ReadFromFile(std::string const& filename);

   cnb_float    Mass() const;
   cnb_float&   Mass();
   cnb_float    Radius() const;
   cnb_float&   Radius();
   Vector3      Position() const;
   Vector3&     Position();
   Vector3      Velocity() const;
   Vector3&     Velocity();
   std::string  Name() const;
   std::string& Name();

   bool        Compare(Particle3 const&) const;
   std::string ToString() const;

   void      LeapPosition(cnb_float const, Vector3 const&);
   void      LeapVelocity(cnb_float const, Vector3 const&);
   cnb_float KineticEnergy() const;
   cnb_float PotentialEnergy(Particle3 const&) const;
   Vector3   Force(Particle3 const&) const;
   Vector3   Separation(Particle3 const&) const;

   friend std::ostream& operator<<(std::ostream&, Particle3 const&);

private:
   Vector3 position; /* AU */
   Vector3 velocity; /* AU/day */
   cnb_float mass;   /* earth masses */
   cnb_float radius; /* km */
   std::string name;

   bool ReadName(std::string const& file_contents);
   bool ReadRadius(std::string const& file_contents);
   bool ReadMass(std::string const& file_contents);
   bool ReadPosition(std::string const& file_contents);
   bool ReadVelocity(std::string const& file_contents);

   size_t RegexMatchCount(boost::sregex_iterator itr);
};

} // namespace cnb

#endif