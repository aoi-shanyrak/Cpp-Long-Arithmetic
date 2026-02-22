#include "superlong.hpp"

#include <algorithm>
#include <stdexcept>


SuperLong SuperLong::multi256n(size_t shift) const {
  if (shift == 0 || isZero()) {
    return *this;
  }
  SuperLong result(*this);

  result.digits.reserve(result.digits.size() + shift);
  result.digits.insert(result.digits.begin(), shift, 0);

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


SuperLong SuperLong::multiply_simple(const SuperLong& a, const SuperLong& b) {
  SuperLong result;
  result.digits.clear(); 

  result.digits.reserve(a.digits.size() + b.digits.size());

  for (size_t i = 0; i < a.digits.size(); i++) {
    SuperLong temp;
    temp.digits.clear(); 

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
  result.removeLeadingZeros();

  return result;
}


SuperLong SuperLong::multiply_karatsuba(const SuperLong& x, const SuperLong& y) {
  if (x.digits.size() < 32 || y.digits.size() < 32) {
    return multiply_simple(x, y);
  }

  size_t m = std::min(x.digits.size(), y.digits.size()) / 2;

  SuperLong b = x.divid256n(m);
  SuperLong a = x.multi256n(b.digits.size());
  SuperLong d = y.divid256n(m);
  SuperLong c = y.multi256n(d.digits.size());

  SuperLong z0 = multiply_karatsuba(b, d);
  SuperLong z1 = multiply_karatsuba(a + b, c + d);
  SuperLong z2 = multiply_karatsuba(a, c);

  return z2.multi256n(2 * m) + (z1 - z2 - z0).multi256n(m) + z0;
}


SuperLong SuperLong::multiply(const SuperLong& a, const SuperLong& b) {
  SuperLong result = multiply_karatsuba(a, b);
  result.sign = (a.sign == b.sign) ? Sign::Positive : Sign::Negative;
  if (result.isZero()) {
    result.sign = Sign::Positive;
  }
  return result;
}


std::pair<SuperLong, SuperLong> SuperLong::divide_quo_rem(const SuperLong& a, const SuperLong& b) {
  if (b.isZero()) {
    throw std::invalid_argument("Division by zero");
  }
  if (a.isZero()) {
    return {SuperLong(), SuperLong()};
  }

  SuperLong dividend = a, divisor = b;
  dividend.sign = divisor.sign = Sign::Positive;

  if (abscmp(dividend, divisor) < 0) {
    return {SuperLong(), a};
  }

  SuperLong quotient, remainder;
  quotient.digits.clear();  

  for (size_t i = dividend.digits.size(); i-- > 0;) {
    remainder = remainder.multi256n(1) + SuperLong(dividend.digits[i]);

    if (abscmp(remainder, divisor) >= 0) {
      int best = 0;
      int low = 1;
      int high = 255;

      while (low <= high) {
        int mid = low + (high - low) / 2;

        SuperLong product = divisor * static_cast<int64_t>(mid);

        if (abscmp(product, remainder) <= 0) {
          best = mid;
          low = mid + 1;
        } else {
          high = mid - 1;
        }
      }
      quotient.digits.push_back(static_cast<n256>(best));
      remainder = remainder - divisor * static_cast<int64_t>(best);
    } else {
      quotient.digits.push_back(0);
    }
  }
  std::reverse(quotient.digits.begin(), quotient.digits.end());

  quotient.removeLeadingZeros();
  remainder.removeLeadingZeros();

  remainder.sign = a.sign;
  quotient.sign = (a.sign == b.sign) ? Sign::Positive : Sign::Negative;
  return {quotient, remainder};
}


SuperLong SuperLong::operator*(const SuperLong& other) const {
  return multiply(*this, other);
}

SuperLong SuperLong::operator/(const SuperLong& other) const {
  return divide_quo_rem(*this, other).first;
}

SuperLong SuperLong::operator%(const SuperLong& other) const {
  return divide_quo_rem(*this, other).second;
}

SuperLong SuperLong::operator*(int64_t other) const {
  return *this * SuperLong(other);
}

SuperLong SuperLong::operator/(int64_t other) const {
  return *this / SuperLong(other);
}

SuperLong SuperLong::operator%(int64_t other) const {
  return *this % SuperLong(other);
}

