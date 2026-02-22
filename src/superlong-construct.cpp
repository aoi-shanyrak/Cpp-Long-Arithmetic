#include "superlong.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>


SuperLong::SuperLong() : sign(Sign::Positive), digits(1, 0) {}

SuperLong::SuperLong(const SuperLong& other) : sign(other.sign), digits(other.digits) {}

SuperLong::SuperLong(SuperLong&& other) noexcept : sign(other.sign), digits(std::move(other.digits)) {}


SuperLong& SuperLong::operator=(const SuperLong& other) {
  if (this != &other) {
    sign = other.sign;
    digits = other.digits;
  }
  return *this;
}


SuperLong& SuperLong::operator=(SuperLong&& other) noexcept {
  if (this != &other) {
    sign = other.sign;
    digits = std::move(other.digits);
  }
  return *this;
}

SuperLong::SuperLong(const std::string& str) {
  if (str.empty()) {
    throw std::invalid_argument("Input string cannot be empty");
  }

  size_t start = 0;
  if (str[0] == '-') {
    sign = Sign::Negative;
    start = 1;
  } else {
    sign = Sign::Positive;
    if (str[0] == '+') {
      start = 1;
    }
  }
  std::string strc = str.substr(start);

  // Validate that all characters are digits
  if (strc.empty()) {
    throw std::invalid_argument("Input string cannot be just a sign");
  }
  for (char c : strc) {
    if (c < '0' || c > '9') {
      throw std::invalid_argument("Input string contains non-digit characters");
    }
  }

  if (strc == "0") {
    digits.push_back(0);
    sign = Sign::Positive;
    return;
  }

  while (!strc.empty()) {
    n256plus remainder = 0;
    std::string new_str;

    for (size_t i = 0; i < strc.size(); i++) {
      n256plus value = remainder * 10 + (strc[i] - '0');
      if (value / 256 != 0 || i > 0 || !new_str.empty()) {
        new_str += static_cast<char>('0' + (value / 256));
      }
      remainder = value % 256;
    }
    
    digits.push_back(static_cast<n256>(remainder));
    strc = new_str;
  }

  removeLeadingZeros();
}


void SuperLong::initFromUint64(uint64_t num) {
  if (num == 0) {
    digits.push_back(0);
    return;
  }
  while (num > 0) {
    digits.push_back(static_cast<n256>(num % 256));
    num /= 256;
  }
}


SuperLong::SuperLong(int64_t num) {
  if (num == INT64_MIN) {
    sign = Sign::Negative;
    digits = std::vector<n256> {0, 0, 0, 0, 0, 0, 0, 128};  // 2^63 in little-endian
    return;
  }
  if (num < 0) {
    sign = Sign::Negative;
    initFromUint64(static_cast<uint64_t>(-num));
  } else {
    sign = Sign::Positive;
    initFromUint64(static_cast<uint64_t>(num));
  }
}


std::string SuperLong::toString() const {
  if (isZero()) {
    return "0";
  }
  std::string result;
 
  SuperLong temp = *this;
  temp.sign = Sign::Positive;

  while (!temp.isZero()) {
    auto [quotient, remainder] = divide_quo_rem(temp, SuperLong(10LL));
    temp = quotient;
    result += static_cast<char>(remainder.digits[0] + '0');
  }
  if (sign == Sign::Negative) {
    result += '-';
  }
  std::reverse(result.begin(), result.end());
  return result;
}


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

