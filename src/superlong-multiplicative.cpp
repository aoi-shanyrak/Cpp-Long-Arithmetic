#include "superlong.hpp"

#include <stdexcept>


SuperLong SuperLong::multi256n(size_t shift) const {
  if (shift == 0 || isZero()) {
    return *this;
  }
  SuperLong result(*this);

  result.digits.reserve(result.digits.size() + shift);
  result.digits.insert(result.digits.end(), shift, 0);

  return result;
}


SuperLong SuperLong::divid256n(size_t shift) const {
  if (shift == 0 || isZero()) {
    return *this;
  }
  if (shift >= digits.size()) {
    return SuperLong();
  }
  SuperLong result(*this);

  result.digits.erase(result.digits.begin(), result.digits.begin() + shift);
  result.removeLeadingZeros();

  return result;
}


SuperLong SuperLong::multiply(const SuperLong& a, const SuperLong& b) {
  SuperLong result;
  result.digits.reserve(a.digits.size() + b.digits.size());

  for (size_t i = 0; i < a.digits.size(); i++) {
    SuperLong temp;
    for (size_t k = 0; k < i; k++) {
      temp.digits.push_back(0);
    }
    n256plus carry = 0;

    for (size_t j = 0; j < b.digits.size(); j++) {
      n256 digitA = a.digits[i];
      n256 digitB = (j < b.digits.size()) ? b.digits[j] : 0;

      n256plus product = static_cast<n256plus>(digitA) * static_cast<n256plus>(digitB) + carry;

      temp.digits.push_back(static_cast<n256>(product & 0xFF));
      carry = static_cast<n256plus>(product >> 8);
    }
    while (carry > 0) {
      temp.digits.push_back(static_cast<n256>(carry & 0xFF));
      carry >>= 8;
    }
    temp.sign = Sign::Positive;

    result = add(result, temp);
  }
  result.sign = (a.sign == b.sign) ? Sign::Positive : Sign::Negative;

  result.removeLeadingZeros();

  return result;
}


SuperLong SuperLong::divide_quo(const SuperLong& a, const SuperLong& b, bool returnRemainder) {
  if (b.isZero()) {
    throw std::invalid_argument("Division by zero");
  }
  if (a.isZero()) {
    return SuperLong();
  }

  SuperLong dividend = a, divisor = b;
  dividend.sign = divisor.sign = Sign::Positive;

  if (abscmp(dividend, divisor) < 0) {
    return (returnRemainder) ? a : SuperLong();
  }

  SuperLong quotient, remainder;

  for (size_t i = dividend.digits.size(); i-- > 0;) {
    remainder = remainder.multi256n(1) + SuperLong(dividend.digits[i]);

    if (abscmp(remainder, divisor) >= 0) {
      n256 best = 0, low = 1, high = 255;

      while (low <= high) {
        n256 mid = low + (high - low) / 2;

        SuperLong product = divisor * SuperLong(static_cast<n256>(mid));

        if (abscmp(product, remainder) <= 0) {
          best = mid;
          low = mid + 1;
        } else {
          high = mid - 1;
        }
      }
      quotient.digits.push_back(best);
      remainder = remainder - divisor * SuperLong(best);
    } else {
      quotient.digits.push_back(0);
    }
  }

  quotient.removeLeadingZeros();
  remainder.removeLeadingZeros();

  if (returnRemainder) {
    remainder.sign = a.sign;
    return remainder;
  } else {
    quotient.sign = (a.sign == b.sign) ? Sign::Positive : Sign::Negative;
    return quotient;
  }
}
