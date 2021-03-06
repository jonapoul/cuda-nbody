#ifndef CUDANBODY_VECTOR3_H
#define CUDANBODY_VECTOR3_H

#include <vector>
#include "Config.h"

namespace cnb {

class Vector3 {
public:
   Vector3();
   Vector3(cnb_float const element);
   Vector3(cnb_float const XX,
           cnb_float const YY,
           cnb_float const ZZ);
   Vector3(Vector3 const&);
   Vector3& operator=(Vector3 const&);

   cnb_float  X() const;
   cnb_float& X();
   cnb_float  Y() const;
   cnb_float& Y();
   cnb_float  Z() const;
   cnb_float& Z();

   bool        Compare(Vector3 const&) const;
   Vector3     Cross(Vector3 const&) const;
   Vector3     operator/(cnb_float const) const;
   Vector3     operator+(Vector3 const&) const;
   Vector3     operator-(Vector3 const&) const;
   Vector3&    operator+=(const Vector3&);
   Vector3&    operator-=(const Vector3&);
   Vector3     operator-() const;

   friend Vector3 operator*(Vector3 const&, cnb_float const);
   friend Vector3 operator*(cnb_float const, Vector3 const&);

   template<typename T> Vector3& operator*=(const T& val) {
      this->X() *= val;
      this->Y() *= val;
      this->Z() *= val;
      return *this;
   }
   template<typename T> Vector3& operator/=(const T& val) {
      *this *= (1.0 / val);
      return *this;
   }

   cnb_float   Magnitude() const;
   cnb_float   MagSquared() const;
   cnb_float   Dot(Vector3 const&) const;
   std::string ToString() const;

   friend std::ostream& operator<<(std::ostream&, Vector3 const&);

private:
   cnb_float xx;
   cnb_float yy;
   cnb_float zz;
};

} /* namespace cnb */

#endif