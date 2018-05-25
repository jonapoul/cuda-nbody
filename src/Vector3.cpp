#include <math.h>
#include <string>

#include "Config.h"
#include "Vector3.h"
using namespace cnb;

Vector3::Vector3()
      : xx(0), yy(0), zz(0) { }

Vector3::Vector3(cnb_float const element)
      : xx(element), yy(element), zz(element) { }

Vector3::Vector3(cnb_float const XX,
                 cnb_float const YY,
                 cnb_float const ZZ)
      : xx(XX), yy(YY), zz(ZZ) { }

Vector3::Vector3(Vector3 const& v) 
      : xx(v.xx), yy(v.yy), zz(v.zz) { }

cnb_float  Vector3::X() const { return this->xx; }
cnb_float& Vector3::X()       { return this->xx; }
cnb_float  Vector3::Y() const { return this->yy; }
cnb_float& Vector3::Y()       { return this->yy; }
cnb_float  Vector3::Z() const { return this->zz; }
cnb_float& Vector3::Z()       { return this->zz; }

cnb_float Vector3::Magnitude() const {
   return sqrt(MagSquared());
}

cnb_float Vector3::MagSquared() const {
   return X()*X() + Y()*Y() + Z()*Z();
}

cnb_float Vector3::Dot(Vector3 const& that) const {
   return this->X() * that.X()
        + this->Y() * that.Y()
        + this->Z() * that.Z();
}

Vector3 Vector3::Cross(Vector3 const& that) const {
   return Vector3( this->Y()*that.Z() - this->Z()*that.Y(),
                   this->Z()*that.X() - this->X()*that.Z(),
                   this->X()*that.Y() - this->Y()*that.X() );
}

Vector3 Vector3::operator/(cnb_float const factor) const {
   return *this * (1.0 / factor);
}

Vector3 Vector3::operator+(Vector3 const& that) const {
   return Vector3( this->X() + that.X(),
                   this->Y() + that.Y(),
                   this->Z() + that.Z());
}

Vector3& Vector3::operator+=(const Vector3& that) {
   *this = *this + that;
   return *this;
}

Vector3& Vector3::operator-=(const Vector3& that) {
   *this += (-that);
   return *this;
}

Vector3 Vector3::operator-(Vector3 const& that) const {
   return *this + (-that);
}

Vector3 Vector3::operator-() const {
   return Vector3(-this->X(),
                  -this->Y(),
                  -this->Z());
}

Vector3& Vector3::operator=(Vector3 const& that) {
   this->xx = that.X();
   this->yy = that.Y();
   this->zz = that.Z();
   return *this;
}

bool Vector3::Compare(Vector3 const& that) const {
   cnb_float const dx = this->X() - that.X();
   cnb_float const dy = this->Y() - that.Y();
   cnb_float const dz = this->Z() - that.Z();
   return   abs(dx)/X() < CNB_EPSILON
         && abs(dy)/Y() < CNB_EPSILON
         && abs(dz)/Z() < CNB_EPSILON;
}

std::string Vector3::ToString() const {
   size_t const bufsize = 64;
   char const format[] = "%+.6e";
   size_t const smallbufsize = 16;
   char x[smallbufsize], y[smallbufsize], z[smallbufsize];
   snprintf(x, smallbufsize, format, X());
   snprintf(y, smallbufsize, format, Y());
   snprintf(z, smallbufsize, format, Z());
   char buf[bufsize];
   snprintf(buf, bufsize, "%s %s %s", x, y, z);
   return std::string(buf);
}

/* this namepsace scoping is to remove linking errors */
namespace cnb {
std::ostream& operator<<(std::ostream& os,
                         Vector3 const& v) {
   return os << v.ToString();
}

Vector3 operator*(Vector3 const& v, cnb_float const f) {
   return Vector3(v.X() * f,
                  v.Y() * f,
                  v.Z() * f);
}

Vector3 operator*(cnb_float const f, Vector3 const& v) {
   return Vector3(v.X() * f,
                  v.Y() * f,
                  v.Z() * f);
}

} /* namespace cnb */
