#ifndef CUDANBODY_PARTICLE3_H
#define CUDANBODY_PARTICLE3_H

#include <vector>
#include <string>
#include <fstream>

#include "Config.h"
#include "Vector3.h"

namespace cuda_nbody {

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

   bool ReadFromFile(std::ifstream& is);

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
   Vector3 position;
   Vector3 velocity;
   cnb_float mass;
   cnb_float radius;
   std::string name;
};

} // namespace cuda_nbody

#endif