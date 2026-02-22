#include "superlong.hpp"

#include <stdexcept>


void SuperLong::removeLeadingZeros() {
  while (digits.size() > 1 and digits.back() == 0) {
    digits.pop_back();
  }
  if (digits.size() == 1 && digits[0] == 0) {
    sign = Sign::Positive;
  }
}


int SuperLong::abscmp(const SuperLong& a, const SuperLong& b) {
  if (a.digits.size() != b.digits.size()) {
    return (a.digits.size() < b.digits.size()) ? -1 : 1;
  }
  for (size_t i = a.digits.size(); i-- > 0;) {
    if (a.digits[i] != b.digits[i]) {
      return (a.digits[i] < b.digits[i]) ? -1 : 1;
    }
  }
  return 0;
}


SuperLong SuperLong::addAbs(const SuperLong& a, const SuperLong& b) {
  SuperLong result;
  n256plus carry = 0;

  size_t maxSize = std::max(a.digits.size(), b.digits.size());
  result.digits.reserve(maxSize + 1);

  for (size_t i = 0; i < maxSize; ++i) {
    n256 digitA = (i < a.digits.size()) ? a.digits[i] : 0;
    n256 digitB = (i < b.digits.size()) ? b.digits[i] : 0;

    n256plus sum = static_cast<n256plus>(digitA) + static_cast<n256plus>(digitB) + static_cast<n256plus>(carry);

    result.digits.push_back(static_cast<n256>(sum % 256));
    carry = static_cast<n256plus>(sum / 256);
  }
  if (carry > 0) {
    result.digits.push_back(static_cast<n256>(carry));
  }
  result.removeLeadingZeros();

  return result;
}


SuperLong SuperLong::subtractAbs(const SuperLong& a, const SuperLong& b) {
  SuperLong result;
  n256plus borrow = 0;

  result.digits.reserve(a.digits.size());

  for (size_t i = 0; i < a.digits.size(); ++i) {
    n256 digitA = a.digits[i];
    n256 digitB = (i < b.digits.size()) ? b.digits[i] : 0;

    n256plus diff = static_cast<n256plus>(digitA) - static_cast<n256plus>(digitB) - static_cast<n256plus>(borrow);

    if (diff >= 0) {
      result.digits.push_back(static_cast<n256>(diff));
      borrow = 0;
    } else {
      result.digits.push_back(static_cast<n256>(diff + 256));
      borrow = 1;
    }
  }
  result.removeLeadingZeros();

  return result;
}


SuperLong SuperLong::add(const SuperLong& a, const SuperLong& b) {
  if (a.sign == b.sign) {
    SuperLong result = addAbs(a, b);
    result.sign = a.sign;
    return result;

  } else {
    int cmp = abscmp(a, b);

    if (cmp == 0) {
      return SuperLong();

    } else if (cmp > 0) {
      SuperLong result = subtractAbs(a, b);
      result.sign = a.sign;
      return result;

    } else {
      SuperLong result = subtractAbs(b, a);
      result.sign = b.sign;
      return result;
    }
  }
}


SuperLong SuperLong::subtract(const SuperLong& a, const SuperLong& b) {
  // a - (-b) = a + b
  if (a.sign != b.sign) {
    SuperLong result = addAbs(a, b);
    result.sign = a.sign;
    return result;

  } else {
    int cmp = abscmp(a, b);

    if (cmp == 0) {
      return SuperLong();

    } else if (cmp > 0) {
      SuperLong result = subtractAbs(a, b);
      result.sign = a.sign;
      return result;

      // a - b = -(b - a) if |a| < |b|
    } else {
      SuperLong result = subtractAbs(b, a);
      result.negate();
      return result;
    }
  }
}
