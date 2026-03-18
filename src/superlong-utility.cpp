#include "superlong.hpp"

using namespace aoi;

void SuperLong::negate() {
  sign = (sign == Sign::Positive) ? Sign::Negative : Sign::Positive;
}

bool SuperLong::isZero() const {
  return digits.size() == 1 && digits[0] == 0;
}

bool SuperLong::isNegative() const {
  return sign == Sign::Negative;
}

bool SuperLong::isPositive() const {
  return sign == Sign::Positive;
}

void SuperLong::removeLeadingZeros() {
  while (digits.size() > 1 and digits.back() == 0) {
    digits.pop_back();
  }
  if (digits.size() == 1 && digits[0] == 0) {
    sign = Sign::Positive;
  }
}
