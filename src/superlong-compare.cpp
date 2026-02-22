#include "superlong.hpp"


bool SuperLong::operator==(const SuperLong& other) const {
  if (sign != other.sign) {
    return false;
  }
  if (digits.size() != other.digits.size()) {
    return false;
  }
  return digits == other.digits;
}


bool SuperLong::operator<(const SuperLong& other) const {
  if (sign != other.sign) {
    return sign == Sign::Negative;
  }
  
  int cmp = abscmp(*this, other);
  
  if (sign == Sign::Positive) {
    return cmp < 0;
  } else {
    return cmp > 0;
  }
}


bool SuperLong::operator<=(const SuperLong& other) const {
  return *this < other || *this == other;
}


bool SuperLong::operator>(const SuperLong& other) const {
  return !(*this <= other);
}


bool SuperLong::operator>=(const SuperLong& other) const {
  return !(*this < other);
}


bool SuperLong::operator!=(const SuperLong& other) const {
  return !(*this == other);
}
