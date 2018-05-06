#include "Particle3.h"

using namespace cuda_nbody;

Particle3::Particle3() 
   : position({}), velocity({}), mass(0.0), radius(0.0), name("") { }

Particle3::Particle3(Particle3 const& p) {
  this->position = p.position;
  this->velocity = p.velocity;
  this->mass   = p.mass;
  this->radius  = p.radius;
  this->name   = p.name;
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

bool Particle3::ReadFromFile(std::ifstream& is) {
  // do something here
  return true;
}

bool Particle3::Compare(Particle3 const& p) const {
   return this->position.Compare( p.position )
       && this->velocity.Compare( p.velocity )
       && abs(this->mass   - p.mass)   / this->mass   < CNB_EPSILON
       && abs(this->radius - p.radius) / this->radius < CNB_EPSILON;
}

Particle3& Particle3::operator=(Particle3 const& p) {
   this->position = p.position;
   this->velocity = p.velocity;
   this->mass     = p.mass;
   this->radius   = p.radius;
   this->name     = p.name;
   return *this;
}

std::ostream& operator<<(std::ostream& os, Particle3 const& p) {
  std::string const indent(10, ' ');
  os << p.Name() << ":\n";
  os << indent << "Position: " << p.Position().ToString() << '\n';
  os << indent << "Velocity: " << p.Velocity().ToString() << '\n';
  os << indent << "Mass:     " << p.Mass() << '\n';
  os << indent << "Radius:   " << p.Radius() << '\n';
  return os;
}