#include <algorithm>
#include <vector>
using namespace std;

#include "Integrator.h"
#include "Config.h"
#include "Particle3.h"
#include "Vector3.h"
#include "TeeStream.h"
using namespace cnb;

Integrator::Integrator(vector<Particle3> * p,
                       Constants * c)
         : particles(p), constants(c) {
   oldForce.resize(p->size());
   newForce.resize(p->size());
   meanForce.resize(p->size());
}

void Integrator::ResetForces() {
   oldForce = newForce;
   fill(newForce.begin(), newForce.end(), Vector3(0.0));
}

void Integrator::UpdatePositions(cnb_float const dt) {
   for (size_t i = 0; i < particles->size(); ++i) {
      Particle3 * p = &(particles->at(i));
      Vector3 const v   = p->Velocity();
      Vector3 const f   = oldForce[i];
      cnb_float const m = p->Mass();
      p->Position() += v*dt + f*(0.5*dt*dt/m);
   }
}

void Integrator::CalculateForces() {
   /* New force */
   for (size_t i = 0; i < particles->size(); ++i) {
      Particle3 * p1 = &(particles->at(i));
      newForce[i] = 0.0;
      for (size_t j = 0; j < particles->size(); ++j) {
         if (i == j) continue;
         Particle3 * p2 = &(particles->at(j));
         newForce[i] += p1->NetForce(*p2);
      }
   }
   /* Mean force */
   for (size_t i = 0; i < particles->size(); ++i) {
      meanForce[i] = 0.5 * (oldForce[i] + newForce[i]);
   }
}

void Integrator::UpdateVelocities(cnb_float const dt) {
   for (size_t i = 0; i < particles->size(); ++i) {
      Particle3 * p = &(particles->at(i));
      p->Velocity() += meanForce[i] * (dt / p->Mass());
   }
}
